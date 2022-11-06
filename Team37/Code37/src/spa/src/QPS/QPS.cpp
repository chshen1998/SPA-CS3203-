using namespace std;

#include <stdio.h>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <set>
#include <list>

#include "QPS.h"
#include "QueryExtractor.h"
#include "QueryEvaluator.h"
#include "QueryTokenizer.h"
#include "QueryValidator.h"
#include "QueryOptimizer.h"
#include "./Structures/PqlError.h"
#include "./Structures/PqlToken.h"
#include "./Structures/PqlQuery.h"
#include "./Validators/ValidatorUtils.h"
#include "./Types/ErrorType.h"
#include "./Types/TokenType.h"
#include "AST/TNode.h"
#include "PKB/PKB.h"
#include <unordered_map>


/*
* Sets the queryServicer from PKB so that we can make API calls to the PKB when evaluating a query
*/
void QPS::setQueryServicer(shared_ptr<QueryServicer> s) {
    QPS::servicer = s;
}

/*
* Takes in query string input from user, parses the query string then return result from PKB
*/
void QPS::evaluate(string query, list<string>& results) {
    try {
        QueryTokenizer tokenizer = QueryTokenizer(query);
        vector<PqlToken> tokens = tokenizer.tokenize();

        QueryValidator validator = QueryValidator(&tokens);
        PqlError pe = validator.validateQuery();

        if (pe.errorType != ErrorType::NONE)
        {
            results.push_back(errorTypeToStringMap[pe.errorType]);
            cout << pe.message;
            return;
        }

        shared_ptr<PqlQuery> pq_pointer = make_shared<PqlQuery>();

        QueryExtractor extractor = QueryExtractor(&tokens, pq_pointer);
        extractor.extractSemantics();

        QueryOptimizer optimizer = QueryOptimizer(pq_pointer);
        optimizer.optimize();

        QueryEvaluator evaluator = QueryEvaluator(pq_pointer, servicer, results);
        evaluator.evaluate();
    }
    catch (SyntaxError pe) {
        results.push_back("SyntaxError");
        cout << pe.message;
        return;
    }
}
