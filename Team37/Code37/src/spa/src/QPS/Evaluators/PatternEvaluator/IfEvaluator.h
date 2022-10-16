#ifndef SPA_IFEVALUATOR_H
#define SPA_IFEVALUATOR_H

using namespace std;

#include "PatternEvaluator.h"
#include "PKB/QueryServicer.h"
#include "QPS/Types/TokenType.h"
#include "QPS/Structures/PqlQuery.h"
#include "PKB/Types/StmtStmtRelationType.h"
#include "PKB/Types/StmtVarRelationType.h"

class IfEvaluator : public PatternEvaluator {

public:
    IfEvaluator(shared_ptr<QueryServicer> s, unordered_map<string, TokenType>& d) :
    PatternEvaluator(s, d) {};

    vector<vector<string>> evaluateClause(const Clause&, vector<vector<string>>);
};


#endif //SPA_IFEVALUATOR_H
