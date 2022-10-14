#ifndef SPA_WHILEEVALUATOR_H
#define SPA_WHILEEVALUATOR_H

using namespace std;

#include "PatternEvaluator.h"
#include "PKB/QueryServicer.h"

class WhileEvaluator : public PatternEvaluator{

public:
    WhileEvaluator(shared_ptr<QueryServicer> s, unordered_map<string, TokenType>& d) :
        PatternEvaluator(s, d) {};

    vector<vector<string>> evaluateClause(const Clause&, vector<vector<string>>);
};


#endif //SPA_WHILEEVALUATOR_H
