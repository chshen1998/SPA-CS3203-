#pragma once

#ifndef TEAM37_ANDCONDITION_H
#define TEAM37_ANDCONDITION_H

#include "ConditionalExpression.h"

#include <memory>
#include <string>

class AndCondition : public ConditionalExpression, public enable_shared_from_this<AndCondition> {
private:
    shared_ptr<ConditionalExpression> condExpr1 = nullptr;
    shared_ptr<ConditionalExpression> condExpr2 = nullptr;

public:
    AndCondition(
        shared_ptr<TNode> parent,
        shared_ptr<ConditionalExpression> condExpr1,
        shared_ptr<ConditionalExpression> condExpr2);

    /**
     * Get the first conditional expression of a and condition
     *
     * @return conditional expression
     */
    shared_ptr<ConditionalExpression> getConditionalExpression1();

    /**
     * Get the second conditional expression of a and condition
     *
     * @return conditional expression
     */
    shared_ptr<ConditionalExpression> getConditionalExpression2();

    void accept(shared_ptr<ASTVisitor> visitor) override;
};

#endif
