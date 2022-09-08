using namespace std;

#include <string>
#include <set>

#include "QueryEvaluator.h"
#include "QPS.h"
#include "PKB/QueryServicer.h"


QueryEvaluator::QueryEvaluator(PqlQuery pqlQuery) {
    pq = pqlQuery;
}

set<string> QueryEvaluator::CallPKB() {
    QuerySelect();
    return selectResult;
}

void QueryEvaluator::QuerySelect() {
    const string selectSynonym = pq.select;
    const TokenType type = pq.declarations[selectSynonym];
    if (type == TokenType::VARIABLE) {
        // TODO
//    	selectResult = QueryServicer::retrieveAll();
    } else if (type == TokenType::CONSTANT) {
//        selectResult = AllConstants();
    } else if (type == TokenType::STATEMENT) {
//        selectResult = AllStatements();
    } else if (type == TokenType::PROCEDURE) {
//        selectResult = AllProcedures();
    } else {
//        throw "Invalid select token type";
    }
}

