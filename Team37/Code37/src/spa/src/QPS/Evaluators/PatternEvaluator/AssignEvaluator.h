#ifndef TEAM37_ASSIGN_EVALUATOR_H
#define TEAM37_ASSIGN_EVALUATOR_H

using namespace std;

#include "QPS/Structures/PqlQuery.h"
#include "QPS/Types/TokenType.h"
#include "PKB/Types/StmtStmtRelationType.h"
#include "PKB/QueryServicer.h"
#include "QPS/Evaluators/SynonymEvaluator.h"


class AssignEvaluator : public SynonymEvaluator {

public:
    AssignEvaluator(shared_ptr<QueryServicer> s, unordered_map<string, TokenType>& d) :
        SynonymEvaluator(s, d) {};

    vector<vector<string>> evaluateSynonymClause(const Clause&, vector<vector<string>>) override;
};

#endif
