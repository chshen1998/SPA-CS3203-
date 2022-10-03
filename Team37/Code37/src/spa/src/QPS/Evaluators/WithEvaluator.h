#ifndef TEAM37_WITH_EVALUATOR_H
#define TEAM37_WITH_EVALUATOR_H

using namespace std;

#include "QPS/Structures/PqlQuery.h"
#include "QPS/Types/TokenType.h"
#include "GeneralEvaluator.h"
#include "PKB/Types/StmtStmtRelationType.h"
#include "PKB/QueryServicer.h"


extern set<TokenType> doubleAttrTokens;

class WithEvaluator : public GeneralEvaluator {

public:
    WithEvaluator(shared_ptr<QueryServicer> s, unordered_map<string, TokenType>& d) :
        GeneralEvaluator(s, d) {};

    vector<vector<string>> evaluateClause(const Clause&, vector<vector<string>>);
    bool evaluateBooleanClause(const Clause&);
    bool WithEvaluator::addProcName(vector<vector<string>>&, const PqlToken&);
    inline string updatedColumnName(const PqlToken&);
    };

#endif
