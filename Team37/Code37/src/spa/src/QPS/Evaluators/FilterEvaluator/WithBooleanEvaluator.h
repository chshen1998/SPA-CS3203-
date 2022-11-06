#ifndef TEAM37_WITH_EVALUATOR_H
#define TEAM37_WITH_EVALUATOR_H

using namespace std;

#include "QPS/Structures/PqlQuery.h"
#include "QPS/Types/TokenType.h"
#include "QPS/Evaluators/BooleanEvaluator.h"
#include "FilterEvaluator.h"
#include "PKB/Types/StmtStmtRelationType.h"
#include "PKB/QueryServicer.h"


class WithBooleanEvaluator : public BooleanEvaluator {

public:
    WithBooleanEvaluator(shared_ptr<QueryServicer> s, unordered_map<string, TokenType> &d) :
        BooleanEvaluator(s, d) {};

    bool evaluateBooleanClause(shared_ptr<Clause> clause) override {
        return clause->left == clause->right;
    };
};

#endif
