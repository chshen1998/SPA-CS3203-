#ifndef TEAM37_FINAL_EVALUATOR_H
#define TEAM37_FINAL_EVALUATOR_H

using namespace std;

#include "QPS/Structures/PqlQuery.h"
#include "QPS/Types/TokenType.h"
#include "FilterEvaluator.h"
#include "PKB/Types/StmtStmtRelationType.h"
#include "PKB/QueryServicer.h"


class FinalEvaluator : public FilterEvaluator {

public:
    FinalEvaluator(shared_ptr<QueryServicer> s, unordered_map<string, TokenType>& d, PqlQuery& p) :
        FilterEvaluator(s, d), pq(p) {};

    void getFinalResult(list<string>&, vector<vector<string>>&);

private:
    PqlQuery& pq;

    void getAllCombinations(list<string>&);

    void updateFinalTableWithAttrName(vector<vector<string>>&);

    unordered_map<int, int> getTableIndexToSelectIndex(vector<vector<string>>&);


    bool checkIfClauseContainsSelect();

    bool checkIfSelectSynonymIsDoubleAttr(const SelectObject&);

    unordered_map<TokenType, TokenType> attrNameToTokenType = {
        {TokenType::PROCNAME, TokenType::PROCEDURE},
        {TokenType::VARNAME, TokenType::VARIABLE},
        {TokenType::VALUE, TokenType::CONSTANT},
    };
};

#endif