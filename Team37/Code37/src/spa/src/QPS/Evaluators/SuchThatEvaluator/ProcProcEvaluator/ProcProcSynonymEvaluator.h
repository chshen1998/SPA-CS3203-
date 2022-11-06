#ifndef TEAM37_PROC_PROC_SYNONYM_EVALUATOR_H
#define TEAM37_PROC_PROC_EVALUATORTEAM37_PROC_PROC_SYNONYM_EVALUATOR_H_H

using namespace std;

#include "PKB/QueryServicer.h"
#include "ProcProcEvaluator.h"
#include "QPS/Evaluators/SynonymEvaluator.h"
#include "QPS/Structures/PqlQuery.h"

class ProcProcSynonymEvaluator : public SynonymEvaluator, public ProcProcEvaluator {
public:
    ProcProcSynonymEvaluator(shared_ptr<QueryServicer> s, unordered_map<string, TokenType>& d)
        : SynonymEvaluator(s, d) {};

    vector<vector<string>> evaluateSynonymClause(shared_ptr<Clause>, vector<vector<string>>) override;
};

#endif // !TEAM37_PROC_PROC_EVALUATOR_H
