#pragma once

#ifndef TEAM37_RELATIONALFACTOR_H
#define TEAM37_RELATIONALFACTOR_H

#include "../Expression.h"

#include <string>
#include <memory>

class RelationalFactor : public Expression {
public:
    RelationalFactor(shared_ptr<TNode> parent) : Expression(parent) { }
};

#endif
