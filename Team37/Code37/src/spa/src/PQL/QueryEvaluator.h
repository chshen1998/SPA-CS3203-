#pragma once

#include <string>
#include <set>

#include "Parser.h"

#ifndef TEAM37_QUERYEVALUATOR_H
#define TEAM37_QUERYEVALUATOR_H


class QueryEvaluator {
public:
    PqlQuery pq;
    set<string> selectResult;

    QueryEvaluator(PqlQuery pqlQuery);
    set<string> CallPKB();

private:
    void QuerySelect();
};


#endif //TEAM37_QUERYEVALUATOR_H
