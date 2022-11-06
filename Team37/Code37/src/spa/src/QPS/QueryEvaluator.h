#pragma once

#ifndef TEAM37_QUERYEVALUATOR_H
#define TEAM37_QUERYEVALUATOR_H

#include <string>
#include <set>
#include <memory>
#include <list>

#include "PKB/QueryServicer.h"
#include "PKB/Types/StatementType.h"
#include "PKB/Types/StmtStmtRelationType.h"
#include "PKB/Types/StmtVarRelationType.h"
#include "./Structures/PqlError.h"
#include "./Structures/PqlToken.h"
#include "./Structures/PqlQuery.h"
#include "./Types/ErrorType.h"
#include "./Types/TokenType.h"

class QueryEvaluator {
public:
    shared_ptr<PqlQuery> pq;
    shared_ptr<QueryServicer> servicer;
    list<string> &result;

    QueryEvaluator(shared_ptr<PqlQuery> pqlQuery, shared_ptr<QueryServicer> s, list<string> &r);

    void evaluate();
};


#endif //TEAM37_QUERYEVALUATOR_H
