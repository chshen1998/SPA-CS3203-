using namespace std;

#include <stdio.h>
#include <iostream>
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
        vector<PqlToken> tokens = tokenizer.Tokenize();

        QueryValidator validator = QueryValidator(tokens);
        PqlError pe = validator.validateQuery();

        if (pe.errorType != ErrorType::NONE)
        {
            results.push_back(errorTypeToStringMap[pe.errorType]);
            cout << pe.message;
            return;
        }

        QueryExtractor extractor = QueryExtractor(tokens);
        PqlQuery pq = extractor.extractSemantics();

        QueryOptimizer optimizer = QueryOptimizer(pq);
        optimizer.optimize();

        QueryEvaluator evaluator = QueryEvaluator(pq, servicer, results);
        evaluator.evaluate();
    }
    catch (SyntaxError pe) {
        results.push_back("Syntax Error");
        cout << pe.message;
        return;
    }
    catch (SemanticError pe) {
        results.push_back("Semantic Error");
        cout << pe.message;
        return;
    }
}
