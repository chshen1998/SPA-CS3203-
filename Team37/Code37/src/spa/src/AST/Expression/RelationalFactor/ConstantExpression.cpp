#include "ConstantExpression.h"

ConstantExpression::ConstantExpression(
        shared_ptr<TNode> parent,
        int value) : RelationalFactor(parent), value(value) { }

int ConstantExpression::getValue() {
    return this->value;
}
