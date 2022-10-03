#ifndef TEAM37_ASSIGN_EVALUATOR_H
#define TEAM37_ASSIGN_EVALUATOR_H

using namespace std;

#include "QPS/Structures/PqlQuery.h"
#include "QPS/Types/TokenType.h"
#include "PatternEvaluator.h"
#include "PKB/Types/StmtStmtRelationType.h"
#include "PKB/QueryServicer.h"

class AssignEvaluator : public PatternEvaluator {

public:
    AssignEvaluator(shared_ptr<QueryServicer> s, unordered_map<string, TokenType>& d) :
        PatternEvaluator(s, d) {};

    void evaluateClause(const Clause&, vector<vector<string>>&);

private:
    inline bool checkWildCardStringOrString(TokenType type);
};

#endif
