using namespace std;

#include "QPS/Structures/PqlQuery.h"
#include "QPS/Structures/PqlToken.h"
#include "QPS/Types/TokenType.h"
#include "QPS/Evaluators/EvaluatorUtils.h"
#include "ProcVarBooleanEvaluator.h"


bool ProcVarBooleanEvaluator::evaluateBooleanClause(const Clause& clause) {
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
