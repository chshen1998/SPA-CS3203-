#ifndef TEAM37_STMT_STMT_BOOLEAN_EVALUATOR_H
#define TEAM37_STMT_STMT_BOOLEAN_EVALUATOR_H

using namespace std;

#include "QPS/Structures/PqlQuery.h"
#include "QPS/Evaluators/BooleanEvaluator.h"
#include "PKB/Types/StmtStmtRelationType.h"
#include "PKB/QueryServicer.h"
#include "StmtStmtEvaluator.h"

class StmtStmtBooleanEvaluator : public BooleanEvaluator, public StmtStmtEvaluator {
public:
    StmtStmtBooleanEvaluator(shared_ptr<QueryServicer> s, unordered_map<string, TokenType >& d) :
        BooleanEvaluator(s, d) {};

    bool evaluateBooleanClause(shared_ptr<Clause>) override;
};

#endif // !TEAM37_STMT_STMT_BOOLEAN_EVALUATOR_H
