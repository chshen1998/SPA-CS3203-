#ifndef TEAM37_PROC_PROC_BOOLEAN_EVALUATOR_H
#define TEAM37_PROC_PROC_BOOLEAN_EVALUATOR_H

using namespace std;

#include "QPS/Structures/PqlQuery.h"
#include "QPS/Evaluators/BooleanEvaluator.h"
#include "PKB/Types/ProcProcRelationType.h"
#include "PKB/QueryServicer.h"
#include "ProcProcEvaluator.h"


class ProcProcBooleanEvaluator : public BooleanEvaluator, public ProcProcEvaluator {
public:
    ProcProcBooleanEvaluator(shared_ptr<QueryServicer> s, unordered_map<string, TokenType >& d) :
        BooleanEvaluator(s, d) {};

    bool evaluateBooleanClause(shared_ptr<Clause>) override;
};



#endif // !TEAM37_PROC_PROC_EVALUATOR_H
