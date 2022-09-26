using namespace std;

#ifndef TEAM37_QPS_H
#define TEAM37_QPS_H

#include <string>
#include <vector>
#include <unordered_map>
#include <set>
#include <list>
#include <iostream>
#include<memory>

#include "PKB/QueryServicer.h"


class QPS {
    shared_ptr<QueryServicer> servicer;

public:
    void setQueryServicer(shared_ptr<QueryServicer> s);
    void evaluate(string query, list<string> &results);
};


#endif //TEAM37_QPS_H

