#include "RelationalExpression.h"

RelationalExpression::RelationalExpression(
        RelationalOperator opr,
        Expression relFactor1,
        Expression relFactor2) : opr(opr), relFactor1(relFactor1), relFactor2(relFactor2) { }

RelationalOperator RelationalExpression::getOperator() {
    return this->opr;
}

Expression RelationalExpression::getRelFactor1() {
    return this->relFactor1;
}

Expression RelationalExpression::getRelFactor2() {
    return this->relFactor2;
}
