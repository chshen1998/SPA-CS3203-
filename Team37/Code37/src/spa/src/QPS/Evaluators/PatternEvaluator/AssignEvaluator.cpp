using namespace std;

#include "QPS/Structures/PqlQuery.h"
#include "QPS/Structures/PqlToken.h"
#include "QPS/Types/TokenType.h"
#include "AssignEvaluator.h"
#include "../EvaluatorUtils.h"

using namespace EvaluatorUtils;

vector<vector<string>> AssignEvaluator::evaluateSynonymClause(shared_ptr<Clause> clause, vector<vector<string>> intermediate) {
    PqlToken leftArg = clause->left;
    PqlToken rightArg = clause->right;
    TokenType patternType = declarations[clause->clauseType.value];
    StatementType patternStmtType = tokenTypeToStatementType[patternType];
    vector<vector<string>> finalTable;
    bool rightArgWildCardString = rightArg.type == TokenType::WILDCARD_STRING;
    vector<int> allAssignStmtLines = getAllLineNumOfStmtType(patternStmtType);

    // Add in the column header
    finalTable.push_back(vector<string>{clause->clauseType.value});

    if (leftArg.type == TokenType::SYNONYM) {
        // Add Synonym column header
        finalTable[0].push_back(leftArg.value);

        // Synonym - WildCardString/String
        if (rightArg.type != TokenType::WILDCARD) {
            set<int> allStmtWithRightArg = servicer->reverseRetrievePatternMatch(rightArg.value,
                                                                                 rightArgWildCardString);

            for (int lines: allStmtWithRightArg) {
                for (string v: servicer->forwardRetrieveRelation(lines, StmtVarRelationType::MODIFIESSV)) {
                    finalTable.push_back(vector<string>{to_string(lines), v});
                }
            }
        }
            // Synonym - WildCard -->  pattern a (s, _) --> Modifies(_, s)
        else {
            for (int lines: allAssignStmtLines) {
                for (string v: servicer->forwardRetrieveRelation(lines, StmtVarRelationType::MODIFIESSV)) {
                    finalTable.push_back(vector<string>{to_string(lines), v});
                }
            }
        }
    } else {
        vector<int> finalResult;

        // String- WildCardString/String pattern a ("x", "x + y")
        if (leftArg.type == TokenType::STRING && checkWildCardStringOrString(rightArg.type)) {
            set<int> allStmtWithRightArg = servicer->reverseRetrievePatternMatch(rightArg.value,
                                                                                 rightArgWildCardString);
            vector<int> allStmtWithLeftArg = servicer->reverseRetrieveRelation(leftArg.value,
                                                                               StmtVarRelationType::MODIFIESSV);
            vector<int> allStmtWithRightArgV = vector(allStmtWithRightArg.begin(), allStmtWithRightArg.end());
            getLineNumInteresection(finalResult, allStmtWithRightArgV, allStmtWithLeftArg);
        }

        // String - WildCard (Essentially a modifies statement)
        else if (leftArg.type == TokenType::STRING && rightArg.type == TokenType::WILDCARD) {
            vector<int> allStmtWithLeftArg = servicer->reverseRetrieveRelation(leftArg.value,
                                                                               StmtVarRelationType::MODIFIESSV);
            getLineNumInteresection(finalResult, allStmtWithLeftArg, allAssignStmtLines);
        }

        // Wildcard- WildCardString/String
        else if (leftArg.type == TokenType::WILDCARD && checkWildCardStringOrString(rightArg.type)) {
            set<int> allStmtWithRightArg = servicer->reverseRetrievePatternMatch(rightArg.value,
                                                                                 rightArgWildCardString);
            finalResult = vector(allStmtWithRightArg.begin(), allStmtWithRightArg.end());
        }

        // Wildcard - wildcard
        else {
            finalResult = allAssignStmtLines;
        }

        for (int line: finalResult) {
            finalTable.push_back(vector<string>{to_string(line)});
        }
    }

    return JoinTable(finalTable, intermediate);
}
