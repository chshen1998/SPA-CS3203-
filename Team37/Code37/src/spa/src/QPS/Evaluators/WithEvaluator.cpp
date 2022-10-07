using namespace std;

#include "QPS/Structures/PqlQuery.h"
#include "QPS/Structures/PqlToken.h"
#include "QPS/Types/TokenType.h"
#include "WithEvaluator.h"
#include "PKB/Types/StatementType.h"
#include "PKB/Types/StmtVarRelationType.h"
#include "EvaluatorUtils.h"

using namespace EvaluatorUtils;

set<TokenType> doubleAttrTokens = {
    TokenType::CALL, TokenType::PRINT, TokenType::READ
};

vector<vector<string>> WithEvaluator::evaluateClause(const Clause& clause, vector<vector<string>> intermediate)
{
    PqlToken leftArg = clause.left;
    PqlToken rightArg = clause.right;
    vector<vector<string>> finalResult;

    finalResult.push_back(intermediate[0]);

    // Add procName for call, print, and read
    bool isLeftDoubleAttr = WithEvaluator::addProcName(intermediate, leftArg);
    string leftValue = isLeftDoubleAttr ? updatedColumnName(leftArg) : leftArg.value;

    // Two Synonyms - s.procName() = v.procName()
    if (clause.rightAttr.type != TokenType::NONE) {        
        bool isRightDoubleAttr = WithEvaluator::addProcName(intermediate, rightArg);

        // Insert Column headers
        int leftArgIndex = -1;
        int rightArgIndex = -1;

        string rightValue = isRightDoubleAttr ? updatedColumnName(rightArg) : rightArg.value;

        for (int i = 0; i < intermediate[0].size(); i++) {
            if (intermediate[0][i] == leftValue) {
                leftArgIndex = i;
            }

            if (intermediate[0][i] == rightValue) {
                rightArgIndex = i;
            }
        }

        for (int j = 1; j < intermediate.size(); j++) {
            if (intermediate[j][leftArgIndex] == intermediate[j][rightArgIndex]) {
                finalResult.push_back(intermediate[j]);
            }
        }
    }

    // intermediate: c, v (stmtnums, var)
    // One Synonyms - c.procName() = "main"
    else {
        int leftArgIndex = -1;

        for (int i = 0; i < intermediate[0].size(); i++) {
            if (intermediate[0][i] == leftValue) {
                leftArgIndex = i;
            }
        }

        for (int j = 1; j < intermediate.size(); j++) {
            if (intermediate[j][leftArgIndex] == rightArg.value) {
                finalResult.push_back(intermediate[j]);
            }
        }
    }

    return finalResult;
}

/*
* For cases with 1 = 1 or "x" = "x"
*/
bool WithEvaluator::evaluateBooleanClause(const Clause& clause) {
    return clause.left == clause.right;
}

bool WithEvaluator::addProcName(vector<vector<string>>& intermediate, const PqlToken& token) {
    string updated = updatedColumnName(token);
    if (doubleAttrTokens.find(declarations[token.value]) != doubleAttrTokens.end() &&
        find(intermediate[0].begin(), intermediate[0].end(), updated) == intermediate[0].end()) {
        int index = -1;

        // Get the column index of the synonym
        for (int i = 0; i < intermediate[0].size(); i++) {
            if (intermediate[0][i] == token.value) {
                index = i;
            }
        }

        intermediate[0].push_back(updated);

        for (int i = 1; i < intermediate.size(); i++) {
            // We should only get one variable for Call, Print, Read
            intermediate[i].push_back(servicer->forwardRetrieveRelation(stoi(intermediate[i][index]), StmtVarRelationType::USESSV)[0]);
        }

        return true;
    }

    return false;
}

inline string WithEvaluator::updatedColumnName(const PqlToken& token) {
    return token.type == TokenType::CALL ? token.value + ".ProcName" : token.value + ".VarName";
}


