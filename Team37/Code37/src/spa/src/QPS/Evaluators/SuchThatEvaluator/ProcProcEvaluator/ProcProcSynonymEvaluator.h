#ifndef TEAM37_PROC_PROC_SYNONYM_EVALUATOR_H
#define TEAM37_PROC_PROC_EVALUATORTEAM37_PROC_PROC_SYNONYM_EVALUATOR_H_H

using namespace std;

#include "QPS/Structures/PqlQuery.h"
#include "QPS/Evaluators/SynonymEvaluator.h"
#include "PKB/QueryServicer.h"
#include "ProcProcEvaluator.h"

class ProcProcSynonymEvaluator : public SynonymEvaluator, public ProcProcEvaluator {
public:
    ProcProcSynonymEvaluator(shared_ptr<QueryServicer> s, unordered_map<string, TokenType >& d) :
        SynonymEvaluator(s, d) {};

    vector<vector<string>> evaluateSynonymClause(const Clause&, vector<vector<string>>) override;
};



#endif // !TEAM37_PROC_PROC_EVALUATOR_H
