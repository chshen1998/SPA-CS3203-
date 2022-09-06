#include "RelationalExpression.h"

RelationalExpression::RelationalExpression(
        shared_ptr<TNode> parent,
        RelationalOperator opr,
        shared_ptr<RelationalFactor> relFactor1,
        shared_ptr<RelationalFactor> relFactor2)
        : ConditionalExpression(parent), opr(opr), relFactor1(relFactor1), relFactor2(relFactor2) { }

RelationalOperator RelationalExpression::getOperator() {
    return this->opr;
}

shared_ptr<RelationalFactor> RelationalExpression::getRelFactor1() {
    return this->relFactor1;
}

shared_ptr<RelationalFactor> RelationalExpression::getRelFactor2() {
    return this->relFactor2;
}
