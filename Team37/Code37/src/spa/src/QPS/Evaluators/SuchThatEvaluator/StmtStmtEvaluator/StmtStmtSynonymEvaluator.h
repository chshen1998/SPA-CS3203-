#ifndef TEAM37_STMT_STMT_SYNONYM_EVALUATOR_H
#define TEAM37_STMT_STMT_SYNONYM_EVALUATOR_H

using namespace std;

#include "QPS/Structures/PqlQuery.h"
#include "QPS/Evaluators/SynonymEvaluator.h"
#include "PKB/Types/StmtStmtRelationType.h"
#include "PKB/QueryServicer.h"
#include "StmtStmtEvaluator.h"


class StmtStmtSynonymEvaluator : public SynonymEvaluator, public StmtStmtEvaluator {
public:
    StmtStmtSynonymEvaluator(shared_ptr<QueryServicer> s, unordered_map<string, TokenType >& d) :
        SynonymEvaluator(s, d) {};

    vector<vector<string>> evaluateSynonymClause(const Clause&, vector<vector<string>>) override;

private:
    bool precheck(const PqlToken, const PqlToken, const StmtStmtRelationType);
};

#endif // !TEAM37_STMT_STMT_SYNONYM_EVALUATOR_H
