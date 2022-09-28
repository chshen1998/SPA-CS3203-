#include "OperatedExpression.h"
#include "NameExpression.h"
#include "ConstantExpression.h"

OperatedExpression::OperatedExpression(
        shared_ptr<TNode> parent,
        Operator opr,
        shared_ptr<RelationalFactor> relFactor1,
        shared_ptr<RelationalFactor> relFactor2)
        : RelationalFactor(parent), opr(opr), relFactor1(relFactor1), relFactor2(relFactor2) { }

Operator OperatedExpression::getOperator() {
    return this->opr;
}

shared_ptr<RelationalFactor> OperatedExpression::getExpression1() {
    return this->relFactor1;
}

shared_ptr<RelationalFactor> OperatedExpression::getExpression2() {
    return this->relFactor2;
}

void OperatedExpression::accept(shared_ptr<ASTVisitor> visitor) {
    visitor->visitOperatedExpression(shared_from_this());
}

/**
 * Helper method to convert Operator object to string
 * @param opr
 * @return a string representation of the operator
 */
string operatorToString(Operator opr) {
    switch (opr) {
        case ADD:
            return "+";
        case SUBTRACT:
            return "-";
        case DIVIDE:
            return "/";
        case MULTIPLY:
            return "*";
        case MODULO:
            return "%";
    }
}

string OperatedExpression::generateString() {
    // Process relFactor1
    string leftSide = this->relFactor1->generateString();
    // Process relFactor2
    string rightSide = this->relFactor2->generateString();

    string oprString = ::operatorToString(this->opr);

    return "(" + leftSide + " " + oprString + " " + rightSide + ")";
}
