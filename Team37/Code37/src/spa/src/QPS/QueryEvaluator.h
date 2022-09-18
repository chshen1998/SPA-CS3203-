#pragma once

#include <string>
#include <set>
#include <memory>
#include <list>
#include <unordered_map>


#include "PKB/QueryServicer.h"
#include "PKB/Types/StatementType.h"
#include "PKB/Types/StmtStmtRelationType.h"
#include "PKB/Types/StmtVarRelationType.h"
#include "QPS.h"

#ifndef TEAM37_QUERYEVALUATOR_H
#define TEAM37_QUERYEVALUATOR_H

extern unordered_map<TokenType, StatementType> tokenTypeToStatementType;
extern unordered_map<TokenType, StmtVarRelationType> tokenTypeToStmtVarRelationType;
extern unordered_map<TokenType, StmtStmtRelationType> tokenTypeToStmtStmtRelationType;

class QueryEvaluator {
public:
    PqlQuery pq;
    shared_ptr<QueryServicer> servicer;
    list<string>&  result;

    QueryEvaluator(PqlQuery& pqlQuery, shared_ptr<QueryServicer> s, list<string>& r);
    void evaluate();

private:
    void QuerySelect();
    bool evaluateSuchThatClause(vector<vector<string>>&);
    void evaluatePatternClause(vector<vector<string>>&);
    void getResultFromFinalTable(const vector<vector<string>>&);
    vector<vector<string>> joinTwoTable(const vector<vector<string>>& a, size_t columna, const vector<vector<string>>& b, size_t columnb);
    void QueryEvaluator::selectAll(TokenType type);

    inline bool checkIfClauseExists();
    bool checkIfSelectSynonymExistsInClause();
};


#endif //TEAM37_QUERYEVALUATOR_H
