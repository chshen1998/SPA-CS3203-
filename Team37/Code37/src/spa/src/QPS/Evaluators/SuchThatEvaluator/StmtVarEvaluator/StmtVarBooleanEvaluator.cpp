using namespace std;

#include "StmtVarBooleanEvaluator.h"
#include "QPS/Structures/PqlQuery.h"
#include "QPS/Structures/PqlToken.h"
#include "QPS/Types/TokenType.h"

bool StmtVarBooleanEvaluator::evaluateBooleanClause(shared_ptr<Clause> clause)
{
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