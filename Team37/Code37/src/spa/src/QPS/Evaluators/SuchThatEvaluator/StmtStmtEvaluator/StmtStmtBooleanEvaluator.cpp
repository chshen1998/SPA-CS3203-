using namespace std;

#include "QPS/Structures/PqlQuery.h"
#include "QPS/Structures/PqlToken.h"
#include "QPS/Types/TokenType.h"
#include "QPS/Evaluators/EvaluatorUtils.h"
#include "StmtStmtBooleanEvaluator.h"

using namespace EvaluatorUtils;

bool StmtStmtBooleanEvaluator::evaluateBooleanClause(shared_ptr<Clause> clause) {
    PqlToken leftArg = clause->left;
    PqlToken rightArg = clause->right;
    StmtStmtRelationType ss = tokenTypeToStmtStmtRelationType[clause->clauseType.type];

    // StmtNum-StmtNum --> Eg. Follows(5,6) 
    if (leftArg.type == TokenType::STATEMENT_NUM && rightArg.type == TokenType::STATEMENT_NUM) {

        return servicer->retrieveRelation(stoi(leftArg.value), stoi(rightArg.value), ss);

    }

    // WildCard-StmtNum --> Eg. Follows(_,6) 
    if (leftArg.type == TokenType::WILDCARD && rightArg.type == TokenType::STATEMENT_NUM) {
        if (checkIfComputeRelation(ss)) {
            return !servicer->reverseComputeRelation(stoi(rightArg.value), ss).empty();
        } else {
            return !servicer->reverseRetrieveRelation(stoi(rightArg.value), ss).empty();
        }
    }

    // StmtNum-WildCard --> Eg. Follows(5,_) 
    if (leftArg.type == TokenType::STATEMENT_NUM && rightArg.type == TokenType::WILDCARD) {
        if (checkIfComputeRelation(ss)) {
            return !servicer->forwardComputeRelation(stoi(leftArg.value), ss).empty();
        } else {
            return !servicer->forwardRetrieveRelation(stoi(leftArg.value), ss).empty();
        }
    }

    // WildCard-WildCard --> Eg. Follows(_,_) 
    if (leftArg.type == TokenType::WILDCARD && rightArg.type == TokenType::WILDCARD) {
        if (checkIfComputeRelation(ss)) {
            for (shared_ptr<Statement> s: servicer->getAllStmt(STATEMENT)) {
                if (!servicer->forwardComputeRelation(s->getLineNum(), ss).empty()) {
                    return true;
                }
            }
        } else {
            for (shared_ptr<Statement> s: servicer->getAllStmt(STATEMENT)) {
                if (!servicer->forwardRetrieveRelation(s->getLineNum(), ss).empty()) {
                    return true;
                }
            }
        }
    }

    return false;
}