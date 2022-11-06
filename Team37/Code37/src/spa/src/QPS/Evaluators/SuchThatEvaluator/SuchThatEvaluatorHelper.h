using namespace std;

#ifndef TEAM37_SUCH_THAT_EVALUATOR_HELPER_H
#define TEAM37_SUCH_THAT_EVALUATOR_HELPER_H

#include "QPS/Types/TokenType.h"
#include "PKB/Types/StmtStmtRelationType.h"
#include "PKB/Types/ProcProcRelationType.h"
#include "PKB/Types/ProcVarRelationType.h"

namespace SuchThatEvaluatorHelper {
    
    /*
    unordered_map<TokenType, StmtStmtRelationType> tokenTypeToStmtStmtRelationType = {
        {TokenType::FOLLOWS,   StmtStmtRelationType::FOLLOWS},
        {TokenType::FOLLOWS_A, StmtStmtRelationType::FOLLOWSS},
        {TokenType::PARENT,    StmtStmtRelationType::PARENT},
        {TokenType::PARENT_A,  StmtStmtRelationType::PARENTS},
        {TokenType::NEXT,      StmtStmtRelationType::NEXT},
        {TokenType::NEXT_A,    StmtStmtRelationType::NEXTS},
        {TokenType::AFFECTS,   StmtStmtRelationType::AFFECTS},
        {TokenType::AFFECTS_A, StmtStmtRelationType::AFFECTSS},
    };
    */

    /*
    unordered_map<TokenType, ProcProcRelationType> tokenTypeToProcProcRelationType = {
        { TokenType::CALLS, ProcProcRelationType::CALLS },
        { TokenType::CALLS_A, ProcProcRelationType::CALLSS },
    };
    */

    /*
    unordered_map<TokenType, ProcVarRelationType> tokenTypeToProcVarRelationType = {
       { TokenType::USES, ProcVarRelationType::USESPV },
       { TokenType::MODIFIES, ProcVarRelationType::MODIFIESPV },
    };
    */

    /*
    unordered_map<TokenType, StmtVarRelationType> tokenTypeToStmtVarRelationType = {
        { TokenType::USES, StmtVarRelationType::USESSV},
        { TokenType::MODIFIES, StmtVarRelationType::MODIFIESSV },
    };
    */

    /*
    inline bool checkIfComputeRelation(const StmtStmtRelationType ss) {
        return ss == StmtStmtRelationType::AFFECTS ||
            ss == StmtStmtRelationType::AFFECTSS ||
            ss == StmtStmtRelationType::NEXT ||
            ss == StmtStmtRelationType::NEXTS;
    }
    */

};

#endif // !TEAM37_SUCH_THAT_EVALUATOR_HELPER_H


