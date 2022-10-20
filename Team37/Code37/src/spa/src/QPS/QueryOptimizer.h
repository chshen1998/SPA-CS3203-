#pragma once

#ifndef TEAM37_QUERYOPTIMIZER_H
#define TEAM37_QUERYOPTIMIZER_H

#include <memory>
#include <string>
#include <vector>
#include <set>

#include "./Structures/PqlError.h"
#include "./Structures/PqlToken.h"
#include "./Structures/PqlQuery.h"
#include "./Types/ErrorType.h"
#include "./Types/TokenType.h"

class QueryOptimizer {
public:
    PqlQuery pq;
    vector<set<string>> synonymSets;

    QueryOptimizer(PqlQuery& pq);

    void optimize();

private:
    vector<Clause> clauses;

    void groupClauses();

    void sortGroupOrder();
};


#endif //TEAM37_QUERYOPTIMIZER_H
