#ifndef TEAM37_PROC_VAR_SYNONYM_EVALUATOR_H
#define TEAM37_PROC_VAR_SYNONYM_EVALUATOR_H

using namespace std;

#include "QPS/Structures/PqlQuery.h"
#include "QPS/Evaluators/SynonymEvaluator.h"
#include "PKB/QueryServicer.h"
#include "ProcVarEvaluator.h"

class ProcVarSynonymEvaluator : public SynonymEvaluator, public ProcVarEvaluator {
public:
    ProcVarSynonymEvaluator(shared_ptr<QueryServicer> s, unordered_map<string, TokenType >& d) :
        SynonymEvaluator(s, d) {};

    vector<vector<string>> evaluateSynonymClause(const Clause&, vector<vector<string>>) override; 
};

#endif // !TEAM37_PROC_VAR_SYNONYM_EVALUATOR_H
