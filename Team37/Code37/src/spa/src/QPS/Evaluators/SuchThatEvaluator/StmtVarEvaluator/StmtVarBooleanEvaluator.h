#ifndef TEAM37_STMT_VAR_BOOLEAN_EVALUATOR_H
#define TEAM37_STMT_VAR_BOOLEAN_EVALUATOR_H

using namespace std;

#include "QPS/Structures/PqlQuery.h"
#include "QPS/Evaluators/BooleanEvaluator.h"
#include "PKB/Types/ProcProcRelationType.h"
#include "PKB/QueryServicer.h"
#include "StmtVarEvaluator.h"


class StmtVarBooleanEvaluator : public BooleanEvaluator, public StmtVarEvaluator {
public:
    StmtVarBooleanEvaluator(shared_ptr<QueryServicer> s, unordered_map<string, TokenType >& d) :
        BooleanEvaluator(s, d) {};

    bool evaluateBooleanClause(const Clause&) override;
};



#endif // !TEAM37_STMT_VAR_BOOLEAN_EVALUATOR_H
