#include "IfStatement.h"

IfStatement::IfStatement(
        shared_ptr<TNode> parent,
        int lineNum,
        ConditionalExpression condExpr) : Statement(parent, lineNum), condExpr(condExpr) { }

void IfStatement::addThenStatement(shared_ptr<Statement> stmt) {
    this->thenStmtLst.push_back(stmt);
}

void IfStatement::addElseStatement(shared_ptr<Statement> stmt) {
    this->elseStmtLst.push_back(stmt);
}

ConditionalExpression IfStatement::getConditionalExpression() {
    return this->condExpr;
}

vector<shared_ptr<Statement>> IfStatement::getThenStatements() {
    return this->thenStmtLst;
}

vector<shared_ptr<Statement>> IfStatement::getElseStatements() {
    return this->elseStmtLst;
}

shared_ptr<TNode> IfStatement::getParent() {
    return Statement::getParent();
}
