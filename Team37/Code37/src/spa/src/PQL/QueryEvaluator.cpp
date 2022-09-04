using namespace std;

#include <string>
#include <vector>

#include "QueryEvaluator.h"
#include "Parser.h"


QueryEvaluator::QueryEvaluator(PqlQuery pqlQuery) {
    pq = pqlQuery;
}

vector<string> QueryEvaluator::CallPKB() {
    QuerySelect();
    return selectResult;
}

void QueryEvaluator::QuerySelect() {
    const string selectSynonym = pq.select;
    const TokenType type = pq.declarations[selectSynonym];
    if (type == TokenType::VARIABLE) {
        selectResult = AllVariables();
    } else if (type == TokenType::CONSTANT) {
        selectResult = AllConstants();
    } else if (type == TokenType::STATEMENT) {
        selectResult = AllStatements();
    } else if (type == TokenType::PROCEDURE) {
        selectResult = AllProcedures();
    } else {
        throw "Invalid select token type";
    }
}

