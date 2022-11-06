#pragma once

#ifndef TEAM37_QUERYEVALUATOR_H
#define TEAM37_QUERYEVALUATOR_H

#include <list>
#include <memory>
#include <set>
#include <string>

#include "./Structures/PqlError.h"
#include "./Structures/PqlQuery.h"
#include "./Structures/PqlToken.h"
#include "./Types/ErrorType.h"
#include "./Types/TokenType.h"
#include "PKB/QueryServicer.h"
#include "PKB/Types/StatementType.h"
#include "PKB/Types/StmtStmtRelationType.h"
#include "PKB/Types/StmtVarRelationType.h"

class QueryEvaluator {
public:
    shared_ptr<PqlQuery> pq;
    shared_ptr<QueryServicer> servicer;
    list<string>& result;

    QueryEvaluator(shared_ptr<PqlQuery> pqlQuery, shared_ptr<QueryServicer> s, list<string>& r);

    void evaluate();
};

#endif // TEAM37_QUERYEVALUATOR_H
