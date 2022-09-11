#pragma once

#include <string>
#include <set>
#include <memory>

#include "../PKB/QueryServicer.h"
#include "QPS.h"

#ifndef TEAM37_QUERYEVALUATOR_H
#define TEAM37_QUERYEVALUATOR_H


class QueryEvaluator {
public:
    PqlQuery pq;
    shared_ptr<QueryServicer> servicer;
    set<string> selectResult;

    QueryEvaluator(PqlQuery pqlQuery, shared_ptr<QueryServicer> s);
    set<string> CallPKB();

private:
    void QuerySelect();
    void evaluateSuchThatClause();
    void evaluatePatternClause();
};


#endif //TEAM37_QUERYEVALUATOR_H
