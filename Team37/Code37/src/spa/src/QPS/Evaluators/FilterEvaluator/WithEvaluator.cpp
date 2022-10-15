using namespace std;

#include "QPS/Structures/PqlQuery.h"
#include "QPS/Structures/PqlToken.h"
#include "QPS/Types/TokenType.h"
#include "WithEvaluator.h"
#include "PKB/Types/StatementType.h"
#include "PKB/Types/StmtVarRelationType.h"
#include "QPS/Evaluators/EvaluatorUtils.h"

using namespace EvaluatorUtils;

vector<vector<string>> WithEvaluator::evaluateClause(const Clause& clause, vector<vector<string>> intermediate)
{
    PqlToken leftArg = clause.left;
    PqlToken rightArg = clause.right;
    vector<vector<string>> finalResult;

    // If initial table is empty
    // Eg. Query only consists of one WITH clause
    if (intermediate.empty()) {
        fillInitialTable(clause, intermediate);
    }

    // Check if current table does not consists of with arguments
    if (find(intermediate[0].begin(), intermediate[0].end(), leftArg.value) == intermediate[0].end()) {
        vector<vector<string>> temp;
        fillInitialTable(clause, temp);

        intermediate = JoinTable(intermediate, temp);
    }

    // Add procName for call, print, and read
    bool isLeftDoubleAttr = WithEvaluator::addProcName(intermediate, leftArg);
    string leftValue = isLeftDoubleAttr ? updatedColumnName(leftArg) : leftArg.value;
    finalResult.push_back(intermediate[0]);

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


void WithEvaluator::fillInitialTable(const Clause& clause, vector<vector<string>>& intermediate) {
    intermediate.push_back(vector<string> { clause.left.value });

    if (clause.right.type == TokenType::SYNONYM) {
        intermediate[0].push_back(clause.right.value);

        for (string left : selectAll(declarations[clause.left.value])) {
            for (string right : selectAll(declarations[clause.right.value])) {
                intermediate.push_back(vector<string> { left, right });
            }
        }
    }
    else {
        for (string left : selectAll(declarations[clause.left.value])) {
            intermediate.push_back(vector<string> { left });
        }
    }
}
