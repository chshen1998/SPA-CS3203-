#ifndef TEAM37_SUCH_THAT_EVALUATOR_H
#define TEAM37_SUCH_THAT_EVALUATOR_H

using namespace std;

#include "../Structures/PqlQuery.h"
#include "../Types/TokenType.h"
#include "GeneralEvaluator.h"
#include "PKB/Types/StmtStmtRelationType.h"
#include "PKB/QueryServicer.h"

class SuchThatEvaluator : public GeneralEvaluator {

public:
    SuchThatEvaluator(shared_ptr<QueryServicer> s, unordered_map<string, TokenType>& d) :
        GeneralEvaluator(s, d) {};

    virtual bool evaluateBooleanClause(const Clause&) {};
};

#endif
