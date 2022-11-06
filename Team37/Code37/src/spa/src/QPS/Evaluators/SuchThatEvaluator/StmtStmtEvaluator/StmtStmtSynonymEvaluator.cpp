using namespace std;

#include "StmtStmtSynonymEvaluator.h"
#include "PKB/Types/StatementType.h"
#include "QPS/Evaluators/EvaluatorUtils.h"
#include "QPS/Structures/PqlQuery.h"
#include "QPS/Structures/PqlToken.h"
#include "QPS/Types/TokenType.h"

using namespace EvaluatorUtils;

vector<vector<string>>
StmtStmtSynonymEvaluator::evaluateSynonymClause(shared_ptr<Clause> clause, vector<vector<string>> intermediate)
{
    PqlToken leftArg = clause->left;
    PqlToken rightArg = clause->right;
    StmtStmtRelationType ss = tokenTypeToStmtStmtRelationType[clause->clauseType.type];

    if (!precheck(leftArg, rightArg, ss)) {
        return vector<vector<string>> {};
    }

    vector<vector<string>> finalTable;
    vector<int> finalResult;
    vector<int> intermediateStmtLines;

    // Synonym-Synonym --> Eg. Follows(s1, s2)
    if (leftArg.type == TokenType::SYNONYM && rightArg.type == TokenType::SYNONYM) {
        finalTable.push_back(vector<string> { leftArg.value, rightArg.value });

        StatementType st1 = tokenTypeToStatementType[declarations[leftArg.value]];
        StatementType st2 = tokenTypeToStatementType[declarations[rightArg.value]];

        vector<int> allLineNumOfLeftSynonym = getAllLineNumOfStmtType(st1);
        vector<int> allLineNumOfRightSynonym = getAllLineNumOfStmtType(st2);

        // For each s1, we get the forward retrieve, aka possible s2 such that Follows(s1,s2)
        // Intersect possible s2 with all statements of type s2
        // Add combinations of (s1, s2) into table
        for (int leftSynonym : allLineNumOfLeftSynonym) {
            if (checkIfComputeRelation(ss)) {
                intermediateStmtLines = servicer->forwardComputeRelation(leftSynonym, ss);
            } else {
                intermediateStmtLines = servicer->forwardRetrieveRelation(leftSynonym, ss);
            }

            finalResult.clear();
            getLineNumInteresection(finalResult, intermediateStmtLines, allLineNumOfRightSynonym);

            for (int rightSynonym : finalResult) {
                finalTable.push_back(vector<string> { to_string(leftSynonym), to_string(rightSynonym) });
            }
        }
    } else {
        string synonymValue = leftArg.type == TokenType::SYNONYM ? leftArg.value : rightArg.value;
        finalTable.push_back(vector<string> { synonymValue });
        StatementType st = tokenTypeToStatementType[declarations[synonymValue]];
        vector<int> allLineNumOfSynonym = getAllLineNumOfStmtType(st);

        // Synonym-WildCard --> Eg. Follows(s, _)
        if (leftArg.type == TokenType::SYNONYM && rightArg.type == TokenType::WILDCARD) {
            for (int lines : allLineNumOfSynonym) {
                if (checkIfComputeRelation(ss)) {
                    if (!servicer->forwardComputeRelation(lines, ss).empty()) {
                        finalTable.push_back(vector<string> { to_string(lines) });
                    }
                } else if (!servicer->forwardRetrieveRelation(lines, ss).empty()) {
                    finalTable.push_back(vector<string> { to_string(lines) });
                }
            }
        }

        // Wildcard-Synonym --> Eg. Follows(_, s)
        else if (leftArg.type == TokenType::WILDCARD && rightArg.type == TokenType::SYNONYM) {
            for (int lines : allLineNumOfSynonym) {
                if (checkIfComputeRelation(ss)) {
                    if (!servicer->reverseComputeRelation(lines, ss).empty()) {
                        finalTable.push_back(vector<string> { to_string(lines) });
                    }
                } else if (!servicer->reverseRetrieveRelation(lines, ss).empty()) {
                    finalTable.push_back(vector<string> { to_string(lines) });
                }
            }
        } else {

            // Synonym-StmtNum --> Eg. Follows(s, 6)
            if (leftArg.type == TokenType::SYNONYM && rightArg.type == TokenType::STATEMENT_NUM) {
                if (checkIfComputeRelation(ss)) {
                    intermediateStmtLines = servicer->reverseComputeRelation(stoi(rightArg.value), ss);
                } else {
                    intermediateStmtLines = servicer->reverseRetrieveRelation(stoi(rightArg.value), ss);
                }
            }

            // StmtNum-Synonym --> Eg. Follows(6, s)
            else {
                if (checkIfComputeRelation(ss)) {
                    intermediateStmtLines = servicer->forwardComputeRelation(stoi(leftArg.value), ss);
                } else {
                    intermediateStmtLines = servicer->forwardRetrieveRelation(stoi(leftArg.value), ss);
                }
            }

            getLineNumInteresection(finalResult, allLineNumOfSynonym, intermediateStmtLines);

            for (int line : finalResult) {
                finalTable.push_back(vector<string> { to_string(line) });
            }
        }
    }

    // Join With Intermediate table
    return JoinTable(intermediate, finalTable);
}

bool StmtStmtSynonymEvaluator::precheck(const PqlToken leftArg, const PqlToken rightArg, const StmtStmtRelationType ss)
{

    // Follows/Follows*/Parent/Parent* cannot have same left/right arg
    if (ss == StmtStmtRelationType::FOLLOWS || ss == StmtStmtRelationType::FOLLOWSS || ss == StmtStmtRelationType::PARENT || ss == StmtStmtRelationType::PARENTS || ss == StmtStmtRelationType::NEXT) {
        if (leftArg == rightArg) {
            return false;
        }
    }

    if (ss == StmtStmtRelationType::PARENT || ss == StmtStmtRelationType::PARENTS) {
        if (declarations[leftArg.value] == TokenType::ASSIGN) {
            return false;
        }
    }

    if (ss == StmtStmtRelationType::AFFECTS || ss == StmtStmtRelationType::AFFECTSS) {
        if ((leftArg.type == TokenType::SYNONYM && declarations[leftArg.value] != TokenType::ASSIGN) || (rightArg.type == TokenType::SYNONYM && declarations[rightArg.value] != TokenType::ASSIGN)) {
            return false;
        }
    }

    return true;
}