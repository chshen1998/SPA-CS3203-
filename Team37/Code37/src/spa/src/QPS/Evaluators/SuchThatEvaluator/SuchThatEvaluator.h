#ifndef TEAM37_SUCH_THAT_EVALUATOR_H
#define TEAM37_SUCH_THAT_EVALUATOR_H

using namespace std;

#include "QPS/Structures/PqlQuery.h"
#include "QPS/Types/TokenType.h"
#include "../GeneralEvaluator.h"
#include "PKB/Types/StmtStmtRelationType.h"
#include "PKB/QueryServicer.h"

class SuchThatEvaluator : public GeneralEvaluator {

public:
    SuchThatEvaluator(shared_ptr<QueryServicer> s, unordered_map<string, TokenType>& d) :
        GeneralEvaluator(s, d) {};

    virtual vector<vector<string>> evaluateSynonymClause(shared_ptr<Clause>, vector<vector<string>>) = 0;
    virtual bool evaluateBooleanClause(shared_ptr<Clause>) = 0;
};

#endif
