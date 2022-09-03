#include "AssignStatement.h"

using namespace std;

AssignStatement::AssignStatement(
        shared_ptr<TNode> parent,
        int lineNum,
        string varName, Expression expr) : Statement(parent, lineNum), expr(expr) {}

string AssignStatement::getVarName() {
    return this->varName;
}

Expression AssignStatement::getExpression() {
    return this->expr;
}

shared_ptr<TNode> AssignStatement::getParent() {
    return Statement::getParent();
}
