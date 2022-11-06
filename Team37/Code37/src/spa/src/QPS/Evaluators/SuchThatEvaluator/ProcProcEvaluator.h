#ifndef TEAM37_PROC_PROC_EVALUATOR_H
#define TEAM37_PROC_PROC_EVALUATOR_H

using namespace std;

#include <memory>

#include "QPS/Structures/PqlQuery.h"
#include "SuchThatEvaluator.h"
#include "PKB/Types/ProcProcRelationType.h"
#include "PKB/QueryServicer.h"

extern unordered_map<TokenType, ProcProcRelationType> tokenTypeToProcProcRelationType;

class ProcProcEvaluator : public SuchThatEvaluator {
public:
    ProcProcEvaluator(shared_ptr<QueryServicer> s, unordered_map<string, TokenType >& d) :
        SuchThatEvaluator(s, d) {};

    bool evaluateBooleanClause(shared_ptr<Clause>) override;
    vector<vector<string>> evaluateSynonymClause(shared_ptr<Clause>, vector<vector<string>>) override;
};



#endif // !TEAM37_PROC_PROC_EVALUATOR_H
