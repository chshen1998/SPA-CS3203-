#ifndef TEAM37_STMT_VAR_EVALUATOR_H
#define TEAM37_STMT_VAR_EVALUATOR_H

using namespace std;

#include "PKB/QueryServicer.h"
#include "QPS/Evaluators/SynonymEvaluator.h"
#include "QPS/Structures/PqlQuery.h"

class StmtVarEvaluator {
protected:
    unordered_map<TokenType, StmtVarRelationType> tokenTypeToStmtVarRelationType = {
        { TokenType::USES, StmtVarRelationType::USESSV },
        { TokenType::MODIFIES, StmtVarRelationType::MODIFIESSV },
    };
};

#endif // !TEAM37_STMT_VAR_EVALUATOR_H
