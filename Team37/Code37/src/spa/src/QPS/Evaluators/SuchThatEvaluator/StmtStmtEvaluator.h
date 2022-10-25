#ifndef TEAM37_STMT_STMT_EVALUATOR_H
#define TEAM37_STMT_STMT_EVALUATOR_H

using namespace std;

#include "QPS/Structures/PqlQuery.h"
#include "SuchThatEvaluator.h"
#include "PKB/Types/StmtStmtRelationType.h"
#include "PKB/QueryServicer.h"

extern unordered_map<TokenType, StmtStmtRelationType> tokenTypeToStmtStmtRelationType;

class StmtStmtEvaluator : public SuchThatEvaluator {
public:
    StmtStmtEvaluator(shared_ptr<QueryServicer> s, unordered_map<string, TokenType >& d) :
        SuchThatEvaluator(s, d) {};

    bool evaluateBooleanClause(const Clause&) override;
    vector<vector<string>> evaluateSynonymClause(const Clause&, vector<vector<string>>) override;

private:
    inline bool checkIfComputeRelation(StmtStmtRelationType ss) {
        return ss == StmtStmtRelationType::AFFECTS ||
            ss == StmtStmtRelationType::AFFECTSS ||
            ss == StmtStmtRelationType::NEXT ||
            ss == StmtStmtRelationType::NEXTS;
    }
};



#endif // !TEAM37_STMT_STMT_EVALUATOR_H
