#ifndef TEAM37_CONDITIONALEXPRESSION_H
#define TEAM37_CONDITIONALEXPRESSION_H

#include "AST/Expression/Expression.h"

class ConditionalExpression : public Expression {
public:
    ConditionalExpression(shared_ptr<TNode> parent) : Expression(parent) { }
};

#endif
