#ifndef TEAM37_SYNONYM_EVALUATOR_H
#define TEAM37_SYNONYM_EVALUATOR_H

using namespace std;

#include "PKB/QueryServicer.h"
#include "PKB/Types/StmtStmtRelationType.h"
#include "QPS/Structures/PqlQuery.h"

class SynonymEvaluator {

public:
    SynonymEvaluator(shared_ptr<QueryServicer> s, unordered_map<string, TokenType>& d)
        : servicer(s)
        , declarations(d) {};

    virtual ~SynonymEvaluator() {}; // destructor for polymorphism

    virtual vector<vector<string>> evaluateSynonymClause(shared_ptr<Clause>, vector<vector<string>>) = 0;

protected:
    shared_ptr<QueryServicer> servicer;
    unordered_map<string, TokenType>& declarations;

    vector<int> getAllLineNumOfStmtType(const StatementType);
};

#endif
