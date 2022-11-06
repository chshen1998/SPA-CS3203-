#ifndef SPA_IFEVALUATOR_H
#define SPA_IFEVALUATOR_H

using namespace std;

#include "PKB/QueryServicer.h"
#include "QPS/Types/TokenType.h"
#include "QPS/Evaluators/SynonymEvaluator.h"
#include "QPS/Structures/PqlQuery.h"

class IfEvaluator : public SynonymEvaluator {

public:
    IfEvaluator(shared_ptr<QueryServicer> s, unordered_map<string, TokenType>& d) :
        SynonymEvaluator(s, d) {};

    vector<vector<string>> evaluateSynonymClause(const Clause&, vector<vector<string>>) override;
};


#endif //SPA_IFEVALUATOR_H
