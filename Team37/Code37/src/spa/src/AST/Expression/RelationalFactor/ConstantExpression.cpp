#include "ConstantExpression.h"

ConstantExpression::ConstantExpression(
        shared_ptr<TNode> parent,
        int value) : RelationalFactor(parent), value(value) { }

int ConstantExpression::getValue() {
    return this->value;
}

void ConstantExpression::accept(shared_ptr<ASTVisitor> visitor) {
    visitor->visitConstantExpression(shared_from_this());
}

