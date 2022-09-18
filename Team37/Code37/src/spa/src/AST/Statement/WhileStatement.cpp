#include "WhileStatement.h"

WhileStatement::WhileStatement(
        shared_ptr<TNode> parent,
        shared_ptr<ConditionalExpression> condExpr
        ) : Statement(parent), condExpr(condExpr) { }

void WhileStatement::addStatement(shared_ptr<Statement> stmt) {
    WhileStatement::stmtLst.push_back(stmt);
}

shared_ptr<ConditionalExpression> WhileStatement::getConditionalExpression() {
    return WhileStatement::condExpr;
}

vector<shared_ptr<Statement> > WhileStatement::getStatements() {
    return WhileStatement::stmtLst;
}

void WhileStatement::accept(shared_ptr<ASTVisitor> visitor) {
    visitor->visitWhileStatement(shared_from_this());
}