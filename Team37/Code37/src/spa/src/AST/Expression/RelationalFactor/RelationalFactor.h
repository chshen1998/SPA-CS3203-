#pragma once

#ifndef TEAM37_RELATIONALFACTOR_H
#define TEAM37_RELATIONALFACTOR_H

#include "../Expression.h"

#include <string>
#include <memory>

class RelationalFactor : public Expression {
public:
    RelationalFactor(shared_ptr<TNode> parent) : Expression(parent) { }

    /**
     * Generates a string representation of the relFactor.
     * The string will be such that any and all ambiguity concerning order of operations is removed
     * (with the help of brackets)
     * @return a string representation of the relFactor
     */
    virtual string generateString() = 0;
};

#endif
