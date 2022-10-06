using namespace std;

#include "QPS/Structures/PqlQuery.h"
#include "QPS/Structures/PqlToken.h"
#include "QPS/Types/TokenType.h"
#include "AssignEvaluator.h"
#include "../EvaluatorUtils.h"

using namespace EvaluatorUtils;

vector<vector<string>> AssignEvaluator::evaluateClause(const Clause& clause, vector<vector<string>> intermediate)
{
    PqlToken leftArg = clause.left;
    PqlToken rightArg = clause.right;
    TokenType patternType = declarations[clause.clauseType.value];
    StatementType patternStmtType = tokenTypeToStatementType[patternType];
    vector<int> finalResult;
    vector<vector<string>> finalTable;

    vector<int> allAssignStmtLines = AssignEvaluator::getAllLineNumOfStmtType(patternStmtType);

    // Add in the column header
    finalTable.push_back(vector<string> {clause.clauseType.value});


    if (leftArg.type == TokenType::SYNONYM) {
        // Add Synonym column header
        finalTable[0].push_back(leftArg.value);

        // Synonym - WildCardString/String
        if (rightArg.type != TokenType::WILDCARD) {
            cout << "SynonymWildCardString/String" << endl;

            bool rightArgWildCardString = rightArg.type == TokenType::WILDCARD_STRING;

            set<int> allStmtWithRightArg = servicer->reverseRetrievePatternMatch(rightArg.value, rightArgWildCardString);

            for (int lines : allStmtWithRightArg) {
                for (string v : servicer->forwardRetrieveRelation(lines, StmtVarRelationType::MODIFIESSV)) {
                    finalTable.push_back(vector<string> {to_string(lines), v});
                }
            }
        }
        // Synonym - WildCard
        else {
            cout << "SynonymWildCard" << endl;
            for (int lines : allAssignStmtLines) {
                for (string v : servicer->forwardRetrieveRelation(lines, StmtVarRelationType::MODIFIESSV)) {
                    cout << to_string(lines) + " "+ v << endl;
                    finalTable.push_back(vector<string> {to_string(lines), v});
                }
            }
        }
    }

    else {
        // String- WildCardString/String pattern a ("x", "x + y")
        if (leftArg.type == TokenType::STRING && checkWildCardStringOrString(rightArg.type)) {
            bool rightArgWildCardString = rightArg.type == TokenType::WILDCARD_STRING;

            set<int> allStmtWithRightArg = servicer->reverseRetrievePatternMatch(rightArg.value, rightArgWildCardString);
            vector<int> allStmtWithLeftArg = servicer->reverseRetrieveRelation(leftArg.value, StmtVarRelationType::MODIFIESSV);

            getLineNumInteresection(finalResult, vector(allStmtWithRightArg.begin(), allStmtWithRightArg.end()), allStmtWithLeftArg);
        }

        // String - WildCard (Essentially a modifies statement)
        else if (leftArg.type == TokenType::STRING && rightArg.type == TokenType::WILDCARD) {
            vector<int> allStmtWithLeftArg = servicer->reverseRetrieveRelation(leftArg.value, StmtVarRelationType::MODIFIESSV);
            getLineNumInteresection(finalResult, allStmtWithLeftArg, allAssignStmtLines);
        }

        // Wildcard- WildCardString/String
        else {
            bool rightArgWildCardString = rightArg.type == TokenType::WILDCARD_STRING;
            
            set<int> allStmtWithRightArg = servicer->reverseRetrievePatternMatch(rightArg.value, rightArgWildCardString); 

            finalResult = vector(allStmtWithRightArg.begin(), allStmtWithRightArg.end());
        }

        for (int line : finalResult) {
            finalTable.push_back(vector<string> { to_string(line) });
        }
    }

    return JoinTable(finalTable, intermediate);
}
