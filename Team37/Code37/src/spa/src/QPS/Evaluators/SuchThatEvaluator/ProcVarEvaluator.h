#ifndef TEAM37_PROC_VAR_EVALUATOR_H
#define TEAM37_PROC_VAR_EVALUATOR_H

using namespace std;

#include "QPS/Structures/PqlQuery.h"
#include "SuchThatEvaluator.h"
#include "PKB/Types/StmtStmtRelationType.h"
#include "PKB/QueryServicer.h"

extern unordered_map<TokenType, ProcVarRelationType> tokenTypeToProcVarRelationType;

class ProcVarEvaluator : public SuchThatEvaluator {
public:
    ProcVarEvaluator(shared_ptr<QueryServicer> s, unordered_map<string, TokenType >& d) :
        SuchThatEvaluator(s, d) {};

    bool evaluateBooleanClause(shared_ptr<Clause>) override;
    vector<vector<string>> evaluateSynonymClause(shared_ptr<Clause>, vector<vector<string>>) override;
};



#endif // !TEAM37_PROC_VAR_EVALUATOR_H
