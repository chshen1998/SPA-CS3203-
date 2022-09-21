#pragma once

#ifndef TEAM37_CONDITIONALEXPRESSION_H
#define TEAM37_CONDITIONALEXPRESSION_H

#include "AST/Expression/Expression.h"

#include <string>
#include <memory>

class ConditionalExpression : public Expression {
public:
    ConditionalExpression(shared_ptr<TNode> parent) : Expression(parent) { }
};

#endif
