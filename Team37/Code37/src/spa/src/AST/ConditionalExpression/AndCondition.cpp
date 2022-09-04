#include "AndCondition.h"

AndCondition::AndCondition(
        ConditionalExpression condExpr1,
        ConditionalExpression condExpr2) : condExpr1(condExpr1), condExpr2(condExpr2) { }

ConditionalExpression AndCondition::getConditionalExpression1() {
    return this->condExpr1;
}

ConditionalExpression AndCondition::getConditionalExpression2() {
    return this->condExpr2;
}
