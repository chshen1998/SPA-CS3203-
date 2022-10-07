using namespace std;

#include "QPS/Structures/PqlQuery.h"
#include "QPS/Structures/PqlToken.h"
#include "QPS/Types/TokenType.h"
#include "StmtStmtEvaluator.h"
#include "../EvaluatorUtils.h"

using namespace EvaluatorUtils;

unordered_map<TokenType, StmtStmtRelationType> tokenTypeToStmtStmtRelationType = {
    { TokenType::FOLLOWS, StmtStmtRelationType::FOLLOWS},
    { TokenType::FOLLOWS_A, StmtStmtRelationType::FOLLOWSS },
    { TokenType::PARENT, StmtStmtRelationType::PARENT },
    { TokenType::PARENT_A, StmtStmtRelationType::PARENTS },
};

bool StmtStmtEvaluator::evaluateBooleanClause(const Clause& clause) {
    PqlToken leftArg = clause.left;
    PqlToken rightArg = clause.right;
    StmtStmtRelationType ss = tokenTypeToStmtStmtRelationType[clause.clauseType.type];

    // StmtNum-StmtNum --> Eg. Follows(5,6) 
    if (leftArg.type == TokenType::STATEMENT_NUM && rightArg.type == TokenType::STATEMENT_NUM) {
        return servicer->retrieveRelation(stoi(leftArg.value), stoi(rightArg.value), ss);
    }

    // WildCard-StmtNum --> Eg. Follows(_,6) 
    if (leftArg.type == TokenType::WILDCARD && rightArg.type == TokenType::STATEMENT_NUM) {
        return !servicer->reverseRetrieveRelation(stoi(rightArg.value), ss).empty();
    }

    // StmtNum-WildCard --> Eg. Follows(5,_) 
    if (leftArg.type == TokenType::STATEMENT_NUM && rightArg.type == TokenType::WILDCARD) {
        return !servicer->reverseRetrieveRelation(stoi(rightArg.value), ss).empty();
    }

    // WildCard-WildCard --> Eg. Follows(_,_) 
    if (leftArg.type == TokenType::WILDCARD && rightArg.type == TokenType::WILDCARD) {
        for (shared_ptr<Statement> s : servicer->getAllStmt(STATEMENT)) {
            if (!servicer->forwardRetrieveRelation(s->getLineNum(), ss).empty()) {
                return true;
            }
        }        
    }

    return false;
}


vector<vector<string>> StmtStmtEvaluator::evaluateSynonymClause(const Clause& clause, vector<vector<string>> intermediate)
{
    PqlToken leftArg = clause.left;
    PqlToken rightArg = clause.right;
    StmtStmtRelationType ss = tokenTypeToStmtStmtRelationType[clause.clauseType.type];

    vector<vector<string>> finalTable;
    vector<int> finalResult;
    vector<int> intermediateStmtLines;
    
    // Synonym-Synonym --> Eg. Follows(s1, s2) 
    if (leftArg.type == TokenType::SYNONYM && rightArg.type == TokenType::SYNONYM) {
        finalTable.push_back(vector<string> { leftArg.value, rightArg.value});

        StatementType st1 = tokenTypeToStatementType[declarations[leftArg.value]];
        StatementType st2 = tokenTypeToStatementType[declarations[rightArg.value]];

        vector<int> allLineNumOfLeftSynonym = getAllLineNumOfStmtType(st1);
        vector<int> allLineNumOfRightSynonym = getAllLineNumOfStmtType(st2);

        // For each s1, we get the forward retrieve, aka possible s2 such that Follows(s1,s2)
        // Intersect possible s2 with all statements of type s2
        // Add combinations of (s1, s2) into table
        for (int leftSynonym : allLineNumOfLeftSynonym) {
            intermediateStmtLines = servicer->forwardRetrieveRelation(leftSynonym, ss);
            finalResult.clear();
            getLineNumInteresection(finalResult, intermediateStmtLines, allLineNumOfRightSynonym);

            for (int rightSynonym : finalResult) {
                finalTable.push_back(vector<string>
                { to_string(leftSynonym), to_string(rightSynonym) });
            }
        }
    }
    else {
        string synonymValue = leftArg.type == TokenType::SYNONYM ? leftArg.value : rightArg.value;
        finalTable.push_back(vector<string> { synonymValue });
        StatementType st = tokenTypeToStatementType[declarations[synonymValue]];
        vector<int> allLineNumOfSynonym = getAllLineNumOfStmtType(st);

            
        // Synonym-WildCard --> Eg. Follows(s, _) 
        if (leftArg.type == TokenType::SYNONYM && rightArg.type == TokenType::WILDCARD) {
            for (int lines : allLineNumOfSynonym) {
                if (!servicer->forwardRetrieveRelation(lines, ss).empty()) {
                    finalTable.push_back(vector<string>{ to_string(lines) });
                }
            }
        }

        // Wildcard-Synonym --> Eg. Follows(_, s) 
        else if (leftArg.type == TokenType::WILDCARD && rightArg.type == TokenType::SYNONYM) {
            for (int lines : allLineNumOfSynonym) {
                if (!servicer->reverseRetrieveRelation(lines, ss).empty()) {
                    finalTable.push_back(vector<string>{ to_string(lines) });
                }
            }
        }

        else {

            // Synonym-StmtNum --> Eg. Follows(s, 6) 
            if (leftArg.type == TokenType::SYNONYM && rightArg.type == TokenType::STATEMENT_NUM) {
                intermediateStmtLines = servicer->reverseRetrieveRelation(stoi(rightArg.value), ss);
            }

            // StmtNum-Synonym --> Eg. Follows(6, s) 
            else {
                intermediateStmtLines = servicer->forwardRetrieveRelation(stoi(leftArg.value), ss);
            }

            getLineNumInteresection(finalResult, allLineNumOfSynonym, intermediateStmtLines);

            for (int line : finalResult) {
                finalTable.push_back(vector<string> { to_string(line) } );
            }
        }
    }

    cout << '\n' << endl;
    EvaluatorUtils::printTable(finalTable);

    // Join With Intermediate table
    return JoinTable(intermediate, finalTable);
}