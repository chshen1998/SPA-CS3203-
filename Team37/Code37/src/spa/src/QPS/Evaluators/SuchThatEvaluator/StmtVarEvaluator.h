#ifndef TEAM37_STMT_VAR_EVALUATOR_H
#define TEAM37_STMT_VAR_EVALUATOR_H

using namespace std;

#include "QPS/Structures/PqlQuery.h"
#include "SuchThatEvaluator.h"
#include "PKB/Types/StmtStmtRelationType.h"
#include "PKB/QueryServicer.h"

extern unordered_map<TokenType, StmtVarRelationType> tokenTypeToStmtVarRelationType;

class StmtVarEvaluator : public SuchThatEvaluator {
public:
    StmtVarEvaluator(shared_ptr<QueryServicer> s, unordered_map<string, TokenType >& d) :
        SuchThatEvaluator(s, d) {};

    bool evaluateBooleanClause(const Clause&) override;
    vector<vector<string>> evaluateSynonymClause(const Clause&, vector<vector<string>>) override;
};



#endif // !TEAM37_STMT_VAR_EVALUATOR_H
