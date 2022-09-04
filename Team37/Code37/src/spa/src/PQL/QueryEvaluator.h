#pragma once

#include <string>
#include <vector>

#include "Parser.h"

#ifndef TEAM37_QUERYEVALUATOR_H
#define TEAM37_QUERYEVALUATOR_H


class QueryEvaluator {
public:
    PqlQuery pq;
    vector<string> selectResult;

    QueryEvaluator(PqlQuery pqlQuery);
    vector<string> CallPKB();

private:
    void QuerySelect();
};


#endif //TEAM37_QUERYEVALUATOR_H
