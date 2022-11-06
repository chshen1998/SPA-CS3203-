using namespace std;

#include "QPS/Structures/PqlQuery.h"
#include "QPS/Structures/PqlToken.h"
#include "QPS/Types/TokenType.h"
#include "QPS/Evaluators/EvaluatorUtils.h"
#include "QPS/Evaluators/SuchThatEvaluator/SuchThatEvaluatorHelper.h"
#include "ProcProcBooleanEvaluator.h"

using namespace SuchThatEvaluatorHelper;
using namespace EvaluatorUtils;

bool ProcProcBooleanEvaluator::evaluateBooleanClause(const Clause& clause) {
    PqlToken leftArg = clause.left;
    PqlToken rightArg = clause.right;
    ProcProcRelationType pp = tokenTypeToProcProcRelationType[clause.clauseType.type];

    // string-string --> Eg. Calls("procedure1", "procedure2") 
    if (leftArg.type == TokenType::STRING && rightArg.type == TokenType::STRING) {
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