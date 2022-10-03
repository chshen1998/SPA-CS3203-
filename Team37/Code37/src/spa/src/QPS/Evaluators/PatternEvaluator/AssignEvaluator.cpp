using namespace std;

#include "QPS/Structures/PqlQuery.h"
#include "QPS/Structures/PqlToken.h"
#include "QPS/Types/TokenType.h"
#include "AssignEvaluator.h"
#include "../EvaluatorUtils.h"

using namespace EvaluatorUtils;

void AssignEvaluator::evaluateClause(const Clause& clause, vector<vector<string>>& intermediate)
{
    PqlToken leftArg = clause.left;
    PqlToken rightArg = clause.right;
    TokenType patternType = declarations[clause.clauseType.value];
    StatementType patternStmtType = tokenTypeToStatementType[patternType];
    vector<int> finalResult;

    vector<int> allAssignStmtLines = AssignEvaluator::getAllLineNumOfStmtType(patternStmtType);

    // String- WildCardString/String pattern a ("x", "x + y")
    if (leftArg.type == TokenType::STRING && checkWildCardStringOrString(rightArg.type)) {

        vector<int> allStmtWithRightArg = servicer->reverseRetrieveRelation(rightArg.value, StmtVarRelationType::USESSV);
        vector<int> allStmtWithLeftArg = servicer->reverseRetrieveRelation(leftArg.value, StmtVarRelationType::MODIFIESSV);
        vector<int> intermediate;

        getLineNumInteresection(intermediate, allStmtWithRightArg, allStmtWithLeftArg);
        getLineNumInteresection(finalResult, intermediate, allAssignStmtLines);

    }

    // String - WildCard
    else if (leftArg.type == TokenType::STRING && rightArg.type == TokenType::WILDCARD) {
        vector<int> allStmtWithLeftArg = servicer->reverseRetrieveRelation(leftArg.value, StmtVarRelationType::MODIFIESSV);
        getLineNumInteresection(finalResult, allStmtWithLeftArg, allAssignStmtLines);
    }

    // Wildcard- WildCardString/String
    else if (leftArg.type == TokenType::WILDCARD && checkWildCardStringOrString(rightArg.type)) {
        vector<int> allStmtWithRightArg = servicer->reverseRetrieveRelation(rightArg.value, StmtVarRelationType::USESSV);

        getLineNumInteresection(finalResult, allStmtWithRightArg, allAssignStmtLines);
    }

    // Synonym - WildCardString/String
    else if (leftArg.type == TokenType::SYNONYM && rightArg.type == TokenType::WILDCARD) {
        vector<int> allStmtWithRightArg = servicer->reverseRetrieveRelation(rightArg.value, StmtVarRelationType::USESSV);
        getLineNumInteresection(finalResult, allStmtWithRightArg, allAssignStmtLines);

        for (int lines : finalResult) {
            for (string v : servicer->forwardRetrieveRelation(lines, StmtVarRelationType::MODIFIESSV)) {
                intermediate.push_back(vector<string> {to_string(lines), v});
            }
        }
    }

    // Synonym - WildCard
    else {
        for (int lines : allAssignStmtLines) {
            for (string v : servicer->forwardRetrieveRelation(lines, StmtVarRelationType::MODIFIESSV)) {
                intermediate.push_back(vector<string> {to_string(lines), v});
            }
        }
    }
}

inline bool checkWildCardStringOrString(TokenType type) {
    type == TokenType::STRING || type == TokenType::WILDCARD_STRING;
};