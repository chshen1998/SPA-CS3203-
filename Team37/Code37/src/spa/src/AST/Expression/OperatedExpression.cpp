#include "OperatedExpression.h"

OperatedExpression::OperatedExpression(
        Operator opr,
        Expression expr1,
        Expression expr2) : opr(opr), expr1(expr1), expr2(expr2) { }

Operator OperatedExpression::getOperator() {
    return this->opr;
}

Expression OperatedExpression::getExpression1() {
    return this->expr1;
}

Expression OperatedExpression::getExpression2() {
    return this->expr2;
}
