using namespace std;

#include "QPS/Structures/PqlQuery.h"
#include "QPS/Structures/PqlToken.h"
#include "QPS/Types/TokenType.h"
#include "WithEvaluator.h"
#include "EvaluatorUtils.h"

using namespace EvaluatorUtils;

void WithEvaluator::evaluateClause(const Clause& clause, vector<vector<string>>& intermediate)
{
    PqlToken leftArg = clause.left;
    PqlToken rightArg = clause.right;
    TokenType patternType = declarations[clause.clauseType.value];
    StatementType patternStmtType = tokenTypeToStatementType[patternType];
    vector<vector<string>> finalResult;

    // Insert Column headers
    finalResult.push_back(intermediate[0]);

    if (clause.leftAttr.type != TokenType::NONE && clause.rightAttr.type != TokenType::NONE) {
        int leftArgIndex = -1;
        int rightArgIndex = -1;

        for (int i = 0; i < intermediate[0].size(); i++) {
            if (intermediate[0][i] == leftArg.value) {
                leftArgIndex = i;
            }

            if (intermediate[0][i] == rightArg.value) {
                rightArgIndex = i;
            }
        }

        for (int j = 1; j < intermediate.size(); j++) {
            if (intermediate[j][leftArgIndex] == intermediate[j][rightArgIndex]) {
                finalResult.push_back(intermediate[j]);
            }
        }
    }

    else {
        int leftArgIndex = -1;

        for (int i = 0; i < intermediate[0].size(); i++) {
            if (intermediate[0][i] == leftArg.value) {
                leftArgIndex = i;
            }
        }

        for (int j = 1; j < intermediate.size(); j++) {
            if (intermediate[j][leftArgIndex] == rightArg.value) {
                finalResult.push_back(intermediate[j]);
            }
        }
    }

    intermediate = finalResult;
}

/*
* For cases with 1 = 1 or "x" = "x"
*/
bool WithEvaluator::evaluateBooleanClause(const Clause& clause)
{
    return clause.left == clause.right;
}

