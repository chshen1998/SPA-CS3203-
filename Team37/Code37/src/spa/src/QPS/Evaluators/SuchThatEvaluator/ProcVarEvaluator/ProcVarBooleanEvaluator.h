#ifndef TEAM37_PROC_VAR_BOOLEAN_EVALUATOR_H
#define TEAM37_PROC_VAR_BOOLEAN_EVALUATOR_H

using namespace std;

#include "QPS/Structures/PqlQuery.h"
#include "QPS/Evaluators/BooleanEvaluator.h"
#include "PKB/Types/StmtStmtRelationType.h"
#include "PKB/QueryServicer.h"
#include "ProcVarEvaluator.h"


class ProcVarBooleanEvaluator : public BooleanEvaluator, public ProcVarEvaluator {
public:
    ProcVarBooleanEvaluator(shared_ptr<QueryServicer> s, unordered_map<string, TokenType >& d) :
        BooleanEvaluator(s, d) {};

    bool evaluateBooleanClause(const Clause&) override;
};

#endif // !TEAM37_PROC_VAR_BOOLEAN_EVALUATOR_H
