#include "OrCondition.h"

OrCondition::OrCondition(
    shared_ptr<TNode> parent,
    shared_ptr<ConditionalExpression> condExpr1,
    shared_ptr<ConditionalExpression> condExpr2)
    : ConditionalExpression(parent)
    , condExpr1(condExpr1)
    , condExpr2(condExpr2)
{
}

shared_ptr<ConditionalExpression> OrCondition::getConditionalExpression1()
{
    return this->condExpr1;
}

shared_ptr<ConditionalExpression> OrCondition::getConditionalExpression2()
{
    return this->condExpr2;
}

void OrCondition::accept(shared_ptr<ASTVisitor> visitor)
{
    visitor->visitOrCondition(shared_from_this());
}
