using namespace std;

#include "QPS/Structures/PqlQuery.h"
#include "QPS/Structures/PqlToken.h"
#include "QPS/Types/TokenType.h"
#include "StmtVarEvaluator.h"
#include "../EvaluatorUtils.h"

using namespace EvaluatorUtils;

unordered_map<TokenType, StmtVarRelationType> tokenTypeToStmtVarRelationType = {
    { TokenType::USES, StmtVarRelationType::USESSV},
    { TokenType::MODIFIES, StmtVarRelationType::MODIFIESSV },
};


bool StmtVarEvaluator::evaluateBooleanClause(shared_ptr<Clause> clause) {
    PqlToken leftArg = clause->left;
    PqlToken rightArg = clause->right;
    StmtVarRelationType sv = tokenTypeToStmtVarRelationType[clause->clauseType.type];

    // StmtNum-StmtNum --> Eg. Uses(5, "x") 
    if (leftArg.type == TokenType::STATEMENT_NUM && rightArg.type == TokenType::STRING) {
        return servicer->retrieveRelation(stoi(leftArg.value), rightArg.value, sv);
    }

    // StmtNum-WildCard --> Eg. Uses(5,_) 
    if (leftArg.type == TokenType::STATEMENT_NUM && rightArg.type == TokenType::WILDCARD) {
        return !servicer->forwardRetrieveRelation(stoi(leftArg.value), sv).empty();
    }

    return false;
}


vector<vector<string>> StmtVarEvaluator::evaluateSynonymClause(shared_ptr<Clause> clause, vector<vector<string>> intermediate)
{
    PqlToken leftArg = clause->left;
    PqlToken rightArg = clause->right;
    StmtVarRelationType sv = tokenTypeToStmtVarRelationType[clause->clauseType.type];
    vector<vector<string>> finalTable;

    string synonymValue = leftArg.type == TokenType::SYNONYM ? leftArg.value : rightArg.value;
    StatementType st = tokenTypeToStatementType[declarations[synonymValue]];
    vector<int> allLineNumOfSynonym = getAllLineNumOfStmtType(st);

    // Synonym-Synonym --> Eg. Uses(s, v) 
    if (leftArg.type == TokenType::SYNONYM && rightArg.type == TokenType::SYNONYM) {
        finalTable.push_back(vector<string>{ leftArg.value, rightArg.value });

        for (int line : allLineNumOfSynonym) {
            for (string v : servicer->forwardRetrieveRelation(line, sv)) {
                finalTable.push_back(vector<string> { to_string(line), v });
            }
        }
    }
    else {
        finalTable.push_back(vector<string>{ synonymValue });
           
        // Synonym-WildCard --> Eg. Uses(s, _) 
        if (leftArg.type == TokenType::SYNONYM && rightArg.type == TokenType::WILDCARD) {
            for (int lines : allLineNumOfSynonym) {
                if (!servicer->forwardRetrieveRelation(lines, sv).empty()) {
                    finalTable.push_back(vector<string>{ to_string(lines) });
                }
            }
        }
        
        // Synonym-string --> Eg. Uses(s, "x") 
        else if (leftArg.type == TokenType::SYNONYM && rightArg.type == TokenType::STRING) {
                vector<int> finalResult;
                vector<int> intermediateStmtLines = servicer->reverseRetrieveRelation(rightArg.value, sv);
                getLineNumInteresection(finalResult, allLineNumOfSynonym, intermediateStmtLines);

                for (int i : finalResult) {
                    finalTable.push_back(vector<string> { to_string(i) });
                }
        }

        // StmtNum-Synonym --> Eg. Modifies(6, v) 
        else if (leftArg.type == TokenType::STATEMENT_NUM && rightArg.type == TokenType::SYNONYM) {
            vector<string> intermediateVariables = servicer->forwardRetrieveRelation(stoi(leftArg.value), sv);
            for (string v : intermediateVariables) {
                finalTable.push_back(vector<string> { v });
            }
        }

        else {}

    }

    return JoinTable(finalTable, intermediate);
}