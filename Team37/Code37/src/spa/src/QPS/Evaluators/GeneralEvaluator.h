#ifndef TEAM37_GENERAL_EVALUATOR_H
#define TEAM37_GENERAL_EVALUATOR_H

using namespace std;

#include "../Structures/PqlQuery.h"
#include "PKB/Types/StmtStmtRelationType.h"
#include "PKB/QueryServicer.h"


extern unordered_map<TokenType, StatementType> tokenTypeToStatementType;

class GeneralEvaluator {

public:
    GeneralEvaluator(shared_ptr<QueryServicer> s, unordered_map<string, TokenType>& d) :
        servicer(s), declarations(d) {};

    virtual ~GeneralEvaluator() {}; // destructor for polymorphism

    virtual void evaluateSynonymClause(const Clause&, vector<vector<string>>& intermediate) {};

protected:
    shared_ptr<QueryServicer> servicer;
    unordered_map<string, TokenType>& declarations;

    void getAllLineNumOfStmtType(vector<int>&, const StatementType);
};


#endif
