#pragma once

#ifndef TEAM37_QUERYOPTIMIZER_H
#define TEAM37_QUERYOPTIMIZER_H

#include <memory>
#include <set>
#include <string>
#include <vector>

#include "./Structures/PqlError.h"
#include "./Structures/PqlQuery.h"
#include "./Structures/PqlToken.h"
#include "./Types/ErrorType.h"
#include "./Types/TokenType.h"

extern set<TokenType> designAbstractionsA;

class QueryOptimizer {
public:
    shared_ptr<PqlQuery> pq;
    vector<set<string>> synonymSets;

    QueryOptimizer(shared_ptr<PqlQuery> pq);

    void optimize();

private:
    void groupClauses();

    void sortGroupOrder();

    void sortGroupClauses();

    void quickSort(vector<shared_ptr<Clause>>* group, vector<int>* scores, int left, int right);

    int partition(vector<shared_ptr<Clause>>* group, vector<int>* scores, int left, int right);
};

#endif // TEAM37_QUERYOPTIMIZER_H
