#include "AndCondition.h"

AndCondition::AndCondition(
        shared_ptr<TNode> parent,
        shared_ptr<ConditionalExpression> condExpr1,
        shared_ptr<ConditionalExpression> condExpr2)
        : ConditionalExpression(parent), condExpr1(condExpr1), condExpr2(condExpr2) { }

shared_ptr<ConditionalExpression> AndCondition::getConditionalExpression1() {
    return this->condExpr1;
}

shared_ptr<ConditionalExpression> AndCondition::getConditionalExpression2() {
    return this->condExpr2;
}
