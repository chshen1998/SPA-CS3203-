#ifndef TEAM37_FILTER_EVALUATOR_H
#define TEAM37_FILTER_EVALUATOR_H

using namespace std;

#include "PKB/QueryServicer.h"
#include "PKB/Types/StmtStmtRelationType.h"
#include "QPS/Structures/PqlQuery.h"
#include "QPS/Types/TokenType.h"

class FilterEvaluator {
public:
    FilterEvaluator(shared_ptr<QueryServicer> s, unordered_map<string, TokenType>& d)
        : servicer(s)
        , declarations(d) {};

protected:
    shared_ptr<QueryServicer> servicer;
    unordered_map<string, TokenType>& declarations;

    string updatedColumnName(const PqlToken&);

    vector<string> selectAll(const TokenType);

    bool addAttrName(vector<vector<string>>&, const PqlToken&);

    unordered_set<TokenType> doubleAttrTokens = {
        TokenType::CALL, TokenType::PRINT, TokenType::READ
    };
};

#endif
