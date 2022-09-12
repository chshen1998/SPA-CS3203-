#pragma once

#ifndef TEAM37_CONSTANTEXPRESSION_H
#define TEAM37_CONSTANTEXPRESSION_H

#include "RelationalFactor.h"

class ConstantExpression : public RelationalFactor, public enable_shared_from_this<ConstantExpression> {
private:
    int value;
public:
    ConstantExpression(shared_ptr<TNode> parent, int value);

    /**
     * Get the value in the constant expression
     *
     * @return value of the constant expression
     */
    int getValue();

    void accept(shared_ptr<ASTVisitor> visitor) override;

    bool operator== (const ConstantExpression& other) const{
        return value == other.value;
    };

    bool operator< (const ConstantExpression& other) const{
        return value > other.value;
    };
};

#endif
