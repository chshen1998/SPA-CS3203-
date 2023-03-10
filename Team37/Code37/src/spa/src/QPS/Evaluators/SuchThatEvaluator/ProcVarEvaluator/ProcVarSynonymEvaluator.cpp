using namespace std;

#include "QPS/Evaluators/EvaluatorUtils.h"
#include "QPS/Structures/PqlQuery.h"
#include "QPS/Structures/PqlToken.h"
#include "QPS/Types/TokenType.h"

#include "ProcVarSynonymEvaluator.h"

using namespace EvaluatorUtils;

vector<vector<string>> ProcVarSynonymEvaluator::evaluateSynonymClause(shared_ptr<Clause> clause, vector<vector<string>> intermediate)
{
    PqlToken leftArg = clause->left;
    PqlToken rightArg = clause->right;
    ProcVarRelationType pv = tokenTypeToProcVarRelationType[clause->clauseType.type];
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
                finalTable.push_back(vector<string> { procedure, v });
            }
        }
    }

    // For wildcards --> Uses(p,_)
    else if (leftArg.type == TokenType::SYNONYM && rightArg.type == TokenType::WILDCARD) {
        finalTable.push_back(vector<string> { leftArg.value });

        for (string procedure : allProcedures) {
            if (!servicer->forwardRetrieveRelation(procedure, pv).empty()) {
                finalTable.push_back(vector<string> { procedure });
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