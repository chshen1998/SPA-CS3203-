#ifndef TEAM37_STMT_STMT_EVALUATOR_H
#define TEAM37_STMT_STMT_EVALUATOR_H

using namespace std;

#include "QPS/Structures/PqlQuery.h"
#include "QPS/Evaluators/SynonymEvaluator.h"
#include "PKB/QueryServicer.h"

class StmtStmtEvaluator {
protected:
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

    inline bool checkIfComputeRelation(const StmtStmtRelationType ss) {
        return ss == StmtStmtRelationType::AFFECTS ||
            ss == StmtStmtRelationType::AFFECTSS ||
            ss == StmtStmtRelationType::NEXT ||
            ss == StmtStmtRelationType::NEXTS;
    }
};

#endif // !TEAM37_STMT_STMT_EVALUATOR_H
