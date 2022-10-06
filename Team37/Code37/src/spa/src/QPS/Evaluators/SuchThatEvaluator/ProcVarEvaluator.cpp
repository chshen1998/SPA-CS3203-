using namespace std;

#include "QPS/Structures/PqlQuery.h"
#include "QPS/Structures/PqlToken.h"
#include "QPS/Types/TokenType.h"
#include "ProcVarEvaluator.h"
#include "../EvaluatorUtils.h"

using namespace EvaluatorUtils;

unordered_map<TokenType, ProcVarRelationType> tokenTypeToProcVarRelationType = {
    { TokenType::USES, ProcVarRelationType::USESPV },
    { TokenType::MODIFIES, ProcVarRelationType::MODIFIESPV },
};


bool ProcVarEvaluator::evaluateBooleanClause(const Clause& clause) {
    PqlToken leftArg = clause.left;
    PqlToken rightArg = clause.right;
    ProcVarRelationType pv = tokenTypeToProcVarRelationType[clause.clauseType.type];

    // StmtNum-StmtNum --> Eg. Uses("procedure", "x") 
    if (leftArg.type == TokenType::STRING && rightArg.type == TokenType::STRING) {
        return servicer->retrieveRelation(leftArg.value, rightArg.value, pv);
    }

    // StmtNum-WildCard --> Eg. Uses(5,_) 
    if (leftArg.type == TokenType::STRING && rightArg.type == TokenType::WILDCARD) {
        return !servicer->forwardRetrieveRelation(leftArg.value, pv).empty();
    }

    return false;
}


vector<vector<string>> ProcVarEvaluator::evaluateSynonymClause(const Clause& clause, vector<vector<string>> intermediate)
{
    PqlToken leftArg = clause.left;
    PqlToken rightArg = clause.right;
    ProcVarRelationType pv = tokenTypeToProcVarRelationType[clause.clauseType.type];
    vector<vector<string>> finalTable;
    vector<string> finalResult;
    vector<string> allProcedures;

    for (Procedure p : servicer->getAllProc()) {
        allProcedures.push_back(p.getProcedureName());
    }

    // Synonym-Synonym --> Eg. Uses(p, v) 
    if (leftArg.type == TokenType::SYNONYM && rightArg.type == TokenType::SYNONYM) {
        finalTable.push_back(vector<string> { leftArg.value, rightArg.value });

        for (string procedure : allProcedures) {
            for (string v : servicer->forwardRetrieveRelation(procedure, pv)) {
                finalTable.push_back(vector<string> { procedure , v });
            }
        }
    }

    // For wildcards --> Uses(p,_)
    else if (leftArg.type == TokenType::SYNONYM && rightArg.type == TokenType::WILDCARD) {
        finalTable.push_back(vector<string> { leftArg.value });

        for (string procedure : allProcedures) {
            if (!servicer->forwardRetrieveRelation(procedure, pv).empty()) {
                finalTable.push_back(vector<string>{ procedure });
            }
        }
    }

    // Synonym-string --> Eg. Uses(p, "x") 
    else if (leftArg.type == TokenType::SYNONYM && rightArg.type == TokenType::STRING) {
        finalTable.push_back(vector<string> { leftArg.value });
        vector<string> intermediateStmtLines = servicer->reverseRetrieveRelation(rightArg.value, pv);

        for (string procedure : intermediateStmtLines) {
            finalTable.push_back(vector<string> { procedure });
        }
    }

    // string-Synonym --> Eg. Modifies("procedure", s) 
    else if (leftArg.type == TokenType::STRING && rightArg.type == TokenType::SYNONYM) {
        finalTable.push_back(vector<string> { rightArg.value });

        vector<string> intermediateVariables = servicer->forwardRetrieveRelation(leftArg.value, pv);

        for (string v : intermediateVariables) {
            finalTable.push_back(vector<string> { v });
        }
    }

    return JoinTable(intermediate, finalTable);
}