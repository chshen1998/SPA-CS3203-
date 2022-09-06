#pragma once

#ifndef TEAM37_EXPRESSION_H
#define TEAM37_EXPRESSION_H

#include "../TNode.h"

class Expression : public TNode {
public:
    Expression(shared_ptr<TNode> parent) : TNode(parent) { }
};

#endif
