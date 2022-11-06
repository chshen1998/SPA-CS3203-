#ifndef SPA_WHILEEVALUATOR_H
#define SPA_WHILEEVALUATOR_H

using namespace std;

#include "QPS/Evaluators/SynonymEvaluator.h"
#include "PKB/QueryServicer.h"
#include "QPS/Types/TokenType.h"
#include "QPS/Structures/PqlQuery.h"
#include "PKB/Types/StmtStmtRelationType.h"
#include "PKB/Types/StmtVarRelationType.h"

class WhileEvaluator : public SynonymEvaluator {

public:
    WhileEvaluator(shared_ptr<QueryServicer> s, unordered_map<string, TokenType>& d) :
        SynonymEvaluator(s, d) {};

    vector<vector<string>> evaluateSynonymClause(shared_ptr<Clause>, vector<vector<string>>) override;
};


#endif //SPA_WHILEEVALUATOR_H
