#ifndef TEAM37_FILTER_EVALUATOR_H
#define TEAM37_FILTER_EVALUATOR_H

using namespace std;

#include "QPS/Structures/PqlQuery.h"
#include "QPS/Types/TokenType.h"
#include "../GeneralEvaluator.h"
#include "PKB/Types/StmtStmtRelationType.h"
#include "PKB/QueryServicer.h"

class FilterEvaluator : public GeneralEvaluator {

public:
    FilterEvaluator(shared_ptr<QueryServicer> s, unordered_map<string, TokenType> &d) :
            GeneralEvaluator(s, d) {};

protected:
    string updatedColumnName(const PqlToken &);

    vector<string> selectAll(const TokenType);

    bool addAttrName(vector<vector<string>> &, const PqlToken &);

    unordered_set<TokenType> doubleAttrTokens = {
            TokenType::CALL, TokenType::PRINT, TokenType::READ
    };
};

#endif
