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
#include "./Structures/PqlError.h"
#include "./Structures/PqlToken.h"
#include "./Structures/PqlQuery.h"
#include "./Validators/ValidatorUtils.h"
#include "./Types/ErrorType.h"
#include "./Types/TokenType.h"
#include "AST/TNode.h"
#include "PKB/PKB.h"
#include <unordered_map>


void QPS::setQueryServicer(shared_ptr<QueryServicer> s) {
    QPS::servicer = s;
}

/*
* Takes in query string input from user, parses the query string then return result from PKB
*/
void QPS::evaluate(string query, list<string>& results) {
    
    QueryTokenizer tokenizer = QueryTokenizer(query);
    vector<PqlToken> tokens;
    try
    {
	    tokens = tokenizer.Tokenize();
    } catch (SyntaxError e)
    {
        results.push_back("Syntax Error");
        results.push_back(e.message);
        return;
    } 
    

    QueryValidator validator = QueryValidator(tokens);
    PqlError pe = validator.validateQuery();

    if (pe.errorType != ErrorType::NONE)
    {
        results.push_back(errorTypeToStringMap[pe.errorType]);
        results.push_back(pe.message);
        return;
    }

    QueryExtractor extractor(tokens);
    PqlQuery pq = extractor.extractSemantics();


    QueryEvaluator evaluator = QueryEvaluator(pq, servicer, results);
    evaluator.evaluate();

}
