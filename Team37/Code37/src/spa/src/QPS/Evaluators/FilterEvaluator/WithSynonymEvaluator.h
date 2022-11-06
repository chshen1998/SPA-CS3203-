#ifndef TEAM37_WITH_SYNONYM_EVALUATOR_H
#define TEAM37_WITH_SYNONYM_EVALUATOR_H

using namespace std;

#include "FilterEvaluator.h"
#include "PKB/QueryServicer.h"
#include "PKB/Types/StmtStmtRelationType.h"
#include "QPS/Evaluators/SynonymEvaluator.h"
#include "QPS/Structures/PqlQuery.h"
#include "QPS/Types/TokenType.h"

class WithSynonymEvaluator : public SynonymEvaluator, public FilterEvaluator {

public:
    WithSynonymEvaluator(shared_ptr<QueryServicer> s, unordered_map<string, TokenType>& d)
        : SynonymEvaluator(s, d)
        , FilterEvaluator(s, d) {};

    vector<vector<string>> evaluateSynonymClause(shared_ptr<Clause>, vector<vector<string>>) override;

private:
    void fillInitialTable(shared_ptr<Clause>, vector<vector<string>>&);
};

#endif
