#include "NotCondition.h"

NotCondition::NotCondition(
        shared_ptr<TNode> parent,
        shared_ptr<ConditionalExpression> condExpr)
        : ConditionalExpression(parent), condExpr(condExpr) {}

shared_ptr<ConditionalExpression> NotCondition::getConditionalExpression() {
    return this->condExpr;
}
