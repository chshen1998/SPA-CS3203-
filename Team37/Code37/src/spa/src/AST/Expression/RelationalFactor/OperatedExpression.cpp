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

/**
 * Helper method to split relFactor into its child classes
 * @param relFactor
 * @return a string representation of the relFactor
 */
string generateString(shared_ptr<RelationalFactor> relFactor) {
    shared_ptr<NameExpression> nameExpression;
    shared_ptr<ConstantExpression> constantExpression;
    shared_ptr<OperatedExpression> operatedExpression;

    nameExpression = dynamic_pointer_cast<NameExpression>(relFactor);
    if (nameExpression != nullptr) {
        return nameExpression->generateString();
    }
    constantExpression = dynamic_pointer_cast<ConstantExpression>(relFactor);
    if (constantExpression != nullptr) {
        return constantExpression->generateString();
    }
    operatedExpression = dynamic_pointer_cast<OperatedExpression>(relFactor);
    if (operatedExpression != nullptr) {
        return operatedExpression->generateString();
    }
    return "ERROR ERROR";
}

string OperatedExpression::generateString() {
    // Process relFactor1
    string leftSide = ::generateString(this->relFactor1);
    // Process relFactor2
    string rightSide = ::generateString(this->relFactor2);

    string oprString = ::operatorToString(this->opr);

    return "(" + leftSide + " " + oprString + " " + rightSide + ")";
}
