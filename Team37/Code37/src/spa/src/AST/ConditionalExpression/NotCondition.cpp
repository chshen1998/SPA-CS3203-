#include "NotCondition.h"

NotCondition::NotCondition(ConditionalExpression condExpr) : condExpr(condExpr) {}

ConditionalExpression NotCondition::getConditionalExpression() {
    return this->condExpr;
}
