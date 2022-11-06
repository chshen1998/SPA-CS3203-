#pragma once

#ifndef TEAM37_EXPRESSION_H
#define TEAM37_EXPRESSION_H

#include "../TNode.h"

#include <memory>
#include <string>

class Expression : public TNode {
public:
    Expression(shared_ptr<TNode> parent)
        : TNode(parent)
    {
    }
};

#endif
