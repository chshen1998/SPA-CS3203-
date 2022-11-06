#ifndef TEAM37_PROC_VAR_EVALUATOR_H
#define TEAM37_PROC_VAR_EVALUATOR_H

using namespace std;

#include "QPS/Structures/PqlQuery.h"
#include "QPS/Evaluators/SynonymEvaluator.h"
#include "PKB/QueryServicer.h"

class ProcVarEvaluator {
protected:
    unordered_map<TokenType, ProcVarRelationType> tokenTypeToProcVarRelationType = {
       { TokenType::USES, ProcVarRelationType::USESPV },
       { TokenType::MODIFIES, ProcVarRelationType::MODIFIESPV },
    };
};

#endif // !TEAM37_PROC_VAR_EVALUATOR_H
