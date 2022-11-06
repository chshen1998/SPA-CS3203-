using namespace std;

#include "QPS/Evaluators/EvaluatorUtils.h"
#include "QPS/Structures/PqlQuery.h"
#include "QPS/Structures/PqlToken.h"
#include "QPS/Types/TokenType.h"

#include "ProcProcSynonymEvaluator.h"

using namespace EvaluatorUtils;

vector<vector<string>> ProcProcSynonymEvaluator::evaluateSynonymClause(shared_ptr<Clause> clause, vector<vector<string>> intermediate)
{
    PqlToken leftArg = clause->left;
    PqlToken rightArg = clause->right;
    ProcProcRelationType pp = tokenTypeToProcProcRelationType[clause->clauseType.type];
    vector<vector<string>> finalTable;
    vector<string> finalResult;
    vector<string> allProcedures;

    for (Procedure p : servicer->getAllProc()) {
        allProcedures.push_back(p.getProcedureName());
    }

    // Synonym-Synonym --> Eg. Calls(p1, p2)
    if (leftArg.type == TokenType::SYNONYM && rightArg.type == TokenType::SYNONYM) {
        finalTable.push_back(vector<string> { leftArg.value, rightArg.value });

        for (string procedure1 : allProcedures) {
            for (string procedure2 : servicer->forwardRetrieveRelation(procedure1, pp)) {
                finalTable.push_back(vector<string> { procedure1, procedure2 });
            }
        }
    }

    // For wildcards --> Calls(p,_)
    else if (leftArg.type == TokenType::SYNONYM && rightArg.type == TokenType::WILDCARD) {
        finalTable.push_back(vector<string> { leftArg.value });

        for (string procedure : allProcedures) {
            if (!servicer->forwardRetrieveRelation(procedure, pp).empty()) {
                finalTable.push_back(vector<string> { procedure });
            }
        }
    }

    // For wildcards --> Calls(_,p)
    else if (leftArg.type == TokenType::WILDCARD && rightArg.type == TokenType::SYNONYM) {
        finalTable.push_back(vector<string> { rightArg.value });

        for (string procedure : allProcedures) {
            if (!servicer->reverseRetrieveRelation(procedure, pp).empty()) {
                finalTable.push_back(vector<string> { procedure });
            }
        }
    }

    // Synonym-string --> Eg. Calls(p, "procedure")
    else if (leftArg.type == TokenType::SYNONYM && rightArg.type == TokenType::STRING) {
        finalTable.push_back(vector<string> { leftArg.value });
        vector<string> intermediateStmtLines = servicer->reverseRetrieveRelation(rightArg.value, pp);

        for (string procedure : intermediateStmtLines) {
            finalTable.push_back(vector<string> { procedure });
        }
    }

    // string-Synonym --> Eg. Calls("procedure", p)
    else if (leftArg.type == TokenType::STRING && rightArg.type == TokenType::SYNONYM) {
        finalTable.push_back(vector<string> { rightArg.value });

        vector<string> intermediateVariables = servicer->forwardRetrieveRelation(leftArg.value, pp);

        for (string v : intermediateVariables) {
            finalTable.push_back(vector<string> { v });
        }
    }

    return JoinTable(intermediate, finalTable);
}