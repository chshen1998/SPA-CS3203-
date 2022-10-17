#ifndef TEAM37_WITH_EVALUATOR_H
#define TEAM37_WITH_EVALUATOR_H

using namespace std;

#include "QPS/Structures/PqlQuery.h"
#include "QPS/Types/TokenType.h"
#include "FilterEvaluator.h"
#include "PKB/Types/StmtStmtRelationType.h"
#include "PKB/QueryServicer.h"


class WithEvaluator : public FilterEvaluator {

public:
    WithEvaluator(shared_ptr<QueryServicer> s, unordered_map<string, TokenType> &d) :
        FilterEvaluator(s, d) {};

    vector<vector<string>> evaluateClause(const Clause &, vector<vector<string>>);

    bool evaluateBooleanClause(const Clause &);

private:
    void fillInitialTable(const Clause&, vector<vector<string>>&);

};

#endif
