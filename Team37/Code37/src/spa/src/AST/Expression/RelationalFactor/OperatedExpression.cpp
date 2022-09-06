#include "OperatedExpression.h"

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
