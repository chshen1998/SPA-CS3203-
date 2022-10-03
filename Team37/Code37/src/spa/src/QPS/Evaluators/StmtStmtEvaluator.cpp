using namespace std;

#include "../Structures/PqlQuery.h"
#include "../Structures/PqlToken.h"
#include "../Types/TokenType.h"
#include "StmtStmtEvaluator.h"
#include "EvaluatorUtils.h"

using namespace EvaluatorUtils;


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


void StmtStmtEvaluator::evaluateSynonymClause(const Clause& clause, vector<vector<string>>& intermediate) 
{
    PqlToken leftArg = clause.left;
    PqlToken rightArg = clause.right;
    StmtStmtRelationType ss = tokenTypeToStmtStmtRelationType[clause.clauseType.type];
    vector<int> finalResult;
    vector<int> intermediateStmtLines;
    
    // Synonym-Synonym --> Eg. Follows(s1, s2) 
    if (leftArg.type == TokenType::SYNONYM && rightArg.type == TokenType::SYNONYM) {
        StatementType st1 = tokenTypeToStatementType[declarations[leftArg.value]];
        StatementType st2 = tokenTypeToStatementType[declarations[rightArg.value]];

        vector<int> allLineNumOfLeftSynonym;
        vector<int> allLineNumOfRightSynonym;

        for (shared_ptr<Statement> s : servicer->getAllStmt(st1)) {
            allLineNumOfLeftSynonym.push_back(s->getLineNum());
        }

        for (shared_ptr<Statement> s : servicer->getAllStmt(st2)) {
            allLineNumOfRightSynonym.push_back(s->getLineNum());
        }

        // For each s1, we get the forward retrieve, aka possible s2 such that Follows(s1,s2)
        // Intersect possible s2 with all statements of type s2
        // Add combinations of (s1, s2) into table
        for (int leftSynonym : allLineNumOfLeftSynonym) {
            intermediateStmtLines = servicer->forwardRetrieveRelation(leftSynonym, ss);
            getLineNumInteresection(finalResult, intermediateStmtLines, allLineNumOfRightSynonym);

            for (int rightSynonym : finalResult) {
                intermediate.push_back(vector<string>
                { to_string(leftSynonym), to_string(rightSynonym) });
            }
        }
    }
    else {
        string synonymValue = leftArg.type == TokenType::SYNONYM ? leftArg.value : rightArg.value;
        StatementType st = tokenTypeToStatementType[declarations[synonymValue]];
        vector<int> allLineNumOfSynonym;
        
        for (shared_ptr<Statement> s : servicer->getAllStmt(st)) {
            allLineNumOfSynonym.push_back(s->getLineNum());
        }

            
        // Synonym-WildCard --> Eg. Follows(s, _) 
        if (leftArg.type == TokenType::SYNONYM && rightArg.type == TokenType::WILDCARD) {
            for (int lines : allLineNumOfSynonym) {
                if (!servicer->forwardRetrieveRelation(lines, ss).empty()) {
                    intermediate.push_back(vector<string>{ to_string(lines) });
                }
            }
        }

        // Wildcard-Synonym --> Eg. Follows(_, s) 
        else if (leftArg.type == TokenType::WILDCARD && rightArg.type == TokenType::SYNONYM) {
            for (int lines : allLineNumOfSynonym) {
                if (!servicer->reverseRetrieveRelation(lines, ss).empty()) {
                    intermediate.push_back(vector<string>{ to_string(lines) });
                }
            }
        }
        
        // Synonym-StmtNum --> Eg. Follows(s, 6) 
        else if (leftArg.type == TokenType::SYNONYM && rightArg.type == TokenType::STATEMENT_NUM) {
            intermediateStmtLines = servicer->reverseRetrieveRelation(stoi(rightArg.value), ss);
        }

        // StmtNum-Synonym --> Eg. Follows(6, s) 
        else {
            intermediateStmtLines = servicer->forwardRetrieveRelation(stoi(leftArg.value), ss);
        }


        getLineNumInteresection(finalResult, allLineNumOfSynonym, intermediateStmtLines);
            
        // Join With Intermediate table
    }
    

}