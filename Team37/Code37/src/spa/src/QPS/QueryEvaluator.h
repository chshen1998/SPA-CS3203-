#pragma once

#include <string>
#include <set>
#include <memory>
#include <list>


#include "../PKB/QueryServicer.h"
#include "QPS.h"

#ifndef TEAM37_QUERYEVALUATOR_H
#define TEAM37_QUERYEVALUATOR_H

unordered_map<TokenType, StatementType> tokenTypeTostatementType;

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
    void QueryEvaluator::selectAll(TokenType type);

    inline bool checkIfClauseExists();
    bool checkIfSelectSynonymExistsInClause();
};


#endif //TEAM37_QUERYEVALUATOR_H
