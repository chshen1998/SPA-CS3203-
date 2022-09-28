#pragma once

#ifndef TEAM37_CONSTANTEXPRESSION_H
#define TEAM37_CONSTANTEXPRESSION_H

#include "RelationalFactor.h"

#include <string>
#include <memory>

class ConstantExpression : public RelationalFactor, public enable_shared_from_this<ConstantExpression> {
private:
    int value = 0;
public:
    ConstantExpression(shared_ptr<TNode> parent, int value);

    /**
     * Get the value in the constant expression
     *
     * @return value of the constant expression
     */
    int getValue();

    void accept(shared_ptr<ASTVisitor> visitor) override;

    /**
     * Generates a string representing the ConstantExpression
     * @return a string representation of the ConstantExpression
     */
    string generateString() override;

    bool operator==(const ConstantExpression &other) const {
        return value == other.value;
    };

    bool operator<(const ConstantExpression &other) const {
        return value > other.value;
    };
};

#endif
