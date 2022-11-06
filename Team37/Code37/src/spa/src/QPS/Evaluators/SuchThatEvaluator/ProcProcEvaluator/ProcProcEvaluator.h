#ifndef TEAM37_PROC_PROC_EVALUATOR_H
#define TEAM37_PROC_PROC_EVALUATOR_H

using namespace std;

#include "QPS/Structures/PqlQuery.h"
#include "QPS/Evaluators/SynonymEvaluator.h"
#include "PKB/QueryServicer.h"

class ProcProcEvaluator {
protected:
    unordered_map<TokenType, ProcProcRelationType> tokenTypeToProcProcRelationType = {
        { TokenType::CALLS, ProcProcRelationType::CALLS },
        { TokenType::CALLS_A, ProcProcRelationType::CALLSS },
    };
};



#endif // !TEAM37_PROC_PROC_SYNONYM_EVALUATOR_H
