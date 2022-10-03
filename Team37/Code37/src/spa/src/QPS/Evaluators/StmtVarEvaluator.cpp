using namespace std;

#include "../Structures/PqlQuery.h"
#include "../Structures/PqlToken.h"
#include "../Types/TokenType.h"
#include "StmtVarEvaluator.h"
#include "EvaluatorUtils.h"

using namespace EvaluatorUtils;


bool StmtVarEvaluator::evaluateBooleanClause(const Clause& clause) {
    PqlToken leftArg = clause.left;
    PqlToken rightArg = clause.right;
    StmtVarRelationType sv = tokenTypeToStmtVarRelationType[clause.clauseType.type];

    // StmtNum-StmtNum --> Eg. Uses(5, "x") 
    if (leftArg.type == TokenType::STATEMENT_NUM && rightArg.type == TokenType::STATEMENT_NUM) {
        return servicer->retrieveRelation(stoi(leftArg.value), rightArg.value, sv);
    }

    // StmtNum-WildCard --> Eg. Uses(5,_) 
    if (leftArg.type == TokenType::STATEMENT_NUM && rightArg.type == TokenType::WILDCARD) {
        return !servicer->forwardRetrieveRelation(stoi(leftArg.value), sv).empty();
    }

    return false;
}


void StmtVarEvaluator::evaluateSynonymClause(const Clause& clause, vector<vector<string>>& intermediate) 
{
    PqlToken leftArg = clause.left;
    PqlToken rightArg = clause.right;
    StmtVarRelationType sv = tokenTypeToStmtVarRelationType[clause.clauseType.type];
    vector<int> allLineNumOfSynonym;
    StatementType st;

    
    // Synonym-Synonym --> Eg. Uses(s, v) 
    if (leftArg.type == TokenType::SYNONYM && rightArg.type == TokenType::SYNONYM) {
       st = tokenTypeToStatementType[declarations[leftArg.value]];

        for (shared_ptr<Statement> s : servicer->getAllStmt(st)) {
            allLineNumOfSynonym.push_back(s->getLineNum());
        }

        for (int line : allLineNumOfSynonym) {
            for (string v : servicer->forwardRetrieveRelation(line, sv)) {
                intermediate.push_back(vector<string> { to_string(line), v });
            }
        }
    }
    else {
        string synonymValue = leftArg.type == TokenType::SYNONYM ? leftArg.value : rightArg.value;
        st = tokenTypeToStatementType[declarations[synonymValue]];
        
        for (shared_ptr<Statement> s : servicer->getAllStmt(st)) {
            allLineNumOfSynonym.push_back(s->getLineNum());
        }

        // For wildcards
        if (leftArg.type == TokenType::WILDCARD || rightArg.type == TokenType::WILDCARD) {
            
            // Synonym-WildCard --> Eg. Uses(s, _) 
            if (leftArg.type == TokenType::SYNONYM && rightArg.type == TokenType::WILDCARD) {
                for (int lines : allLineNumOfSynonym) {
                    if (!servicer->forwardRetrieveRelation(lines, sv).empty()) {
                        intermediate.push_back(vector<string>{ to_string(lines) });
                    }
                }
            }
            // Throw error due to ambiguity for wildcard-wildcard or wildcard-string
            else {

            }
        }
        
        else {
            // Synonym-string --> Eg. Uses(s, "x") 
            if (leftArg.type == TokenType::SYNONYM && rightArg.type == TokenType::STRING) {
                vector<int> finalResult;
                vector<int> intermediateStmtLines = servicer->reverseRetrieveRelation(rightArg.value, sv);
                getLineNumInteresection(finalResult, allLineNumOfSynonym, intermediateStmtLines);

                for (int i : finalResult) {
                    intermediate.push_back(vector<string> { to_string(i) });
                }
            }

            // StmtNum-Synonym --> Eg. Modifies(6, v) 
            else if (leftArg.type == TokenType::STATEMENT_NUM && rightArg.type == TokenType::SYNONYM) {
                vector<string> intermediateVariables = servicer->forwardRetrieveRelation(stoi(leftArg.value), sv);

                for (string v : intermediateVariables) {
                    intermediate.push_back(vector<string> { v });
                }
            }

            // This occurs if clauses are placed wrongly
            else {}

            
            // Join With Intermediate table
        }
    }

}