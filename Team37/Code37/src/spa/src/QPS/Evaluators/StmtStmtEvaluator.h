#ifndef TEAM37_STMT_STMT_EVALUATOR_H
#define TEAM37_STMT_STMT_EVALUATOR_H

using namespace std;

#include "../Structures/PqlQuery.h"
#include "SuchThatEvaluator.h"
#include "PKB/Types/StmtStmtRelationType.h"
#include "PKB/QueryServicer.h"

unordered_map<TokenType, StmtStmtRelationType> tokenTypeToStmtStmtRelationType = {
    { TokenType::FOLLOWS, StmtStmtRelationType::FOLLOWS},
    { TokenType::FOLLOWS_A, StmtStmtRelationType::FOLLOWSS },
    { TokenType::PARENT, StmtStmtRelationType::PARENT },
    { TokenType::PARENT_A, StmtStmtRelationType::PARENTS },
};


class StmtStmtEvaluator : public SuchThatEvaluator {
public:
    StmtStmtEvaluator(shared_ptr<QueryServicer> s, unordered_map<string, TokenType >& d) :
        SuchThatEvaluator(s, d) {};

    bool evaluateBooleanClause(const Clause&);
    void evaluateSynonymClause(const Clause&, vector<vector<string>>& intermediate);
};



#endif // !TEAM37_STMT_STMT_EVALUATOR_H
