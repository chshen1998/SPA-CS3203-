#include "ConstantExpression.h"

ConstantExpression::ConstantExpression(int value) : value(value) { }

int ConstantExpression::getValue() {
    return this->value;
}
