using namespace std;

#include "QPS/Structures/PqlQuery.h"
#include "QPS/Structures/PqlToken.h"
#include "QPS/Types/TokenType.h"
#include "ProcProcEvaluator.h"
#include "../EvaluatorUtils.h"

using namespace EvaluatorUtils;

unordered_map<TokenType, ProcProcRelationType> tokenTypeToProcProcRelationType = {
    { TokenType::CALLS, ProcProcRelationType::CALLS },
    { TokenType::CALLS_A, ProcProcRelationType::CALLSS },
};


bool ProcProcEvaluator ::evaluateBooleanClause(const Clause& clause) {
    PqlToken leftArg = clause.left;
    PqlToken rightArg = clause.right;
    ProcProcRelationType pp = tokenTypeToProcProcRelationType[clause.clauseType.type];

    // string-string --> Eg. Calls("procedure1", "procedure2") 
    if (leftArg.type == TokenType::STRING && rightArg.type == TokenType::STRING) {
        cout << leftArg.value << " " << rightArg.value << endl;
        return servicer->retrieveRelation(leftArg.value, rightArg.value, pp);
    }

    // string-WildCard --> Eg. Calls("procedure1",_) 
    if (leftArg.type == TokenType::STRING && rightArg.type == TokenType::WILDCARD) {
        return !servicer->forwardRetrieveRelation(leftArg.value, pp).empty();
    }

    // Wildcard-string --> Eg. Calls(_, "procedure1") 
    if (leftArg.type == TokenType::WILDCARD && rightArg.type == TokenType::STRING) {
        return !servicer->reverseRetrieveRelation(rightArg.value, pp).empty();
    }

    // Wildcard-wildcard --> Eg. Calls(_, _) 
    if (leftArg.type == TokenType::WILDCARD && rightArg.type == TokenType::WILDCARD) {
        for (Procedure p : servicer->getAllProc()) {
            if (!servicer->forwardRetrieveRelation(p.getProcedureName(), pp).empty()) {
                return true;
            }
        }
    }

    return false;
}


vector<vector<string>> ProcProcEvaluator::evaluateSynonymClause(const Clause& clause, vector<vector<string>> intermediate)
{
    PqlToken leftArg = clause.left;
    PqlToken rightArg = clause.right;
    ProcProcRelationType pp = tokenTypeToProcProcRelationType[clause.clauseType.type];
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
                finalTable.push_back(vector<string>{ procedure });
            }
        }
    }

    // For wildcards --> Calls(_,p)
    else if (leftArg.type == TokenType::WILDCARD && rightArg.type == TokenType::SYNONYM) {
        finalTable.push_back(vector<string> { rightArg.value });

        for (string procedure : allProcedures) {
            if (!servicer->reverseRetrieveRelation(procedure, pp).empty()) {
                finalTable.push_back(vector<string>{ procedure });
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