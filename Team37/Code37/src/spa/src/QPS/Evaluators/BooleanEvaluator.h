#ifndef TEAM37_BOOLEAN_EVALUATOR_H
#define TEAM37_BOOLEAN_EVALUATOR_H

using namespace std;

#include "../Structures/PqlQuery.h"
#include "PKB/QueryServicer.h"
#include "PKB/Types/StmtStmtRelationType.h"

class BooleanEvaluator {

public:
    BooleanEvaluator(shared_ptr<QueryServicer> s, unordered_map<string, TokenType>& d)
        : servicer(s)
        , declarations(d) {};

    virtual ~BooleanEvaluator() {}; // destructor for polymorphism

    virtual bool evaluateBooleanClause(shared_ptr<Clause>) = 0;

protected:
    shared_ptr<QueryServicer> servicer;
    unordered_map<string, TokenType>& declarations;
};

#endif
