#ifndef TEAM37_PATTERN_EVALUATOR_H
#define TEAM37_PATTERN_EVALUATOR_H

using namespace std;

#include "QPS/Structures/PqlQuery.h"
#include "QPS/Types/TokenType.h"
#include "../GeneralEvaluator.h"
#include "PKB/Types/StmtStmtRelationType.h"
#include "PKB/QueryServicer.h"

class PatternEvaluator : public GeneralEvaluator {

public:
    PatternEvaluator(shared_ptr<QueryServicer> s, unordered_map<string, TokenType>& d) :
        GeneralEvaluator(s, d) {};

    virtual vector<vector<string>> evaluateClause(const Clause&, vector<vector<string>>) = 0;
};

#endif
