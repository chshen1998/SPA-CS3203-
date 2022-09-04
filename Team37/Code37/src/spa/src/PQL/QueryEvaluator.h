#pragma once

#include <string>
#include <vector>

#include "Pql.h"

#ifndef TEAM37_QUERYEVALUATOR_H
#define TEAM37_QUERYEVALUATOR_H


class QueryEvaluator {
public:
    ParsedQuery pq;
    vector<string> selectResult;

    QueryEvaluator(ParsedQuery parsedQuery);
    vector<string> CallPKB();

private:
    void QuerySelect();
};


#endif //TEAM37_QUERYEVALUATOR_H
