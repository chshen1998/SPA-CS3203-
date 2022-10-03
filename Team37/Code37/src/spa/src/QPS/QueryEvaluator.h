#pragma once

#ifndef TEAM37_QUERYEVALUATOR_H
#define TEAM37_QUERYEVALUATOR_H

#include <string>
#include <set>
#include <memory>
#include <list>
#include <unordered_map>


#include "PKB/QueryServicer.h"
#include "PKB/Types/StatementType.h"
#include "PKB/Types/StmtStmtRelationType.h"
#include "PKB/Types/StmtVarRelationType.h"
#include "./Structures/PqlError.h"
#include "./Structures/PqlToken.h"
#include "./Structures/PqlQuery.h"
#include "./Types/ErrorType.h"
#include "./Types/TokenType.h"

extern set<TokenType> suchThatStmtRefStmtRef;

class QueryEvaluator {
public:
    PqlQuery pq;
    shared_ptr<QueryServicer> servicer;
    list<string> &result;

    QueryEvaluator(PqlQuery &pqlQuery, shared_ptr<QueryServicer> s, list<string> &r);

    void evaluate();

private:
    void getResultFromFinalTable(const vector<vector<string>> &);
    void selectAll(TokenType type);
};


#endif //TEAM37_QUERYEVALUATOR_H
