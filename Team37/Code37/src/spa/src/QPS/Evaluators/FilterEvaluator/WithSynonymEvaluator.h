#ifndef TEAM37_WITH_SYNONYM_EVALUATOR_H
#define TEAM37_WITH_SYNONYM_EVALUATOR_H

using namespace std;

#include "QPS/Structures/PqlQuery.h"
#include "QPS/Types/TokenType.h"
#include "QPS/Evaluators/SynonymEvaluator.h"
#include "FilterEvaluator.h"
#include "PKB/Types/StmtStmtRelationType.h"
#include "PKB/QueryServicer.h"


class WithSynonymEvaluator : public SynonymEvaluator, public FilterEvaluator {

public:
    WithSynonymEvaluator(shared_ptr<QueryServicer> s, unordered_map<string, TokenType>& d) :
        SynonymEvaluator(s, d), FilterEvaluator(s, d) {};

    vector<vector<string>> evaluateSynonymClause(const Clause&, vector<vector<string>>) override;

private:
    void fillInitialTable(const Clause&, vector<vector<string>>&);

};

#endif
