#include "OrCondition.h"

OrCondition::OrCondition(
        ConditionalExpression condExpr1,
        ConditionalExpression condExpr2) : condExpr1(condExpr1), condExpr2(condExpr2) { }

ConditionalExpression OrCondition::getConditionalExpression1() {
    return this->condExpr1;
}

ConditionalExpression OrCondition::getConditionalExpression2() {
    return this->condExpr2;
}
