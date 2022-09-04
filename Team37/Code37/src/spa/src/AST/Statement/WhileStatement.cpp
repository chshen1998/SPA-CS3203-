#include "WhileStatement.h"

WhileStatement::WhileStatement(
        shared_ptr<TNode> parent,
        int lineNum,
        ConditionalExpression condExpr
        ) : Statement(parent, lineNum), condExpr(condExpr) { }

void WhileStatement::addStatement(shared_ptr<Statement> stmt) {
    WhileStatement::stmtLst.push_back(stmt);
}

ConditionalExpression WhileStatement::getConditionalExpression() {
    return WhileStatement::condExpr;
}

vector<shared_ptr<Statement>> WhileStatement::getStatements() {
    return WhileStatement::stmtLst;
}

shared_ptr<TNode> WhileStatement::getParent() {
    return Statement::getParent();
}
