#pragma once

#include <string>
#include <set>
#include <memory>
#include <list>


#include "../PKB/QueryServicer.h"
#include "QPS.h"

#ifndef TEAM37_QUERYEVALUATOR_H
#define TEAM37_QUERYEVALUATOR_H


class QueryEvaluator {
public:
    PqlQuery pq;
    shared_ptr<QueryServicer> servicer;
    list<string>  result;

    QueryEvaluator(PqlQuery pqlQuery, shared_ptr<QueryServicer> s, list<string>& r);
    void evaluate();

private:
    void QuerySelect();
    void evaluateSuchThatClause();
    void evaluatePatternClause();
};


#endif //TEAM37_QUERYEVALUATOR_H
