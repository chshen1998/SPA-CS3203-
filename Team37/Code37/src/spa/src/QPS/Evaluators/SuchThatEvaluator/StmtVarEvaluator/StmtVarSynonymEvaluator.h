#ifndef TEAM37_STMT_VAR_SYNONYM_EVALUATOR_H
#define TEAM37_STMT_VAR_SYNONYM_EVALUATOR_H

using namespace std;

#include "PKB/QueryServicer.h"
#include "PKB/Types/StmtStmtRelationType.h"
#include "QPS/Structures/PqlQuery.h"
#include "StmtVarEvaluator.h"

class StmtVarSynonymEvaluator : public SynonymEvaluator, public StmtVarEvaluator {
public:
    StmtVarSynonymEvaluator(shared_ptr<QueryServicer> s, unordered_map<string, TokenType>& d)
        : SynonymEvaluator(s, d) {};

    vector<vector<string>> evaluateSynonymClause(shared_ptr<Clause>, vector<vector<string>>) override;
};

#endif // !TEAM37_STMT_VAR_SYNONYM_EVALUATOR_H
