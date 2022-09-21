#pragma once

#ifndef TEAM37_ORCONDITION_H
#define TEAM37_ORCONDITION_H

#include "ConditionalExpression.h"

#include <string>
#include <memory>

class OrCondition : public ConditionalExpression, public enable_shared_from_this<OrCondition> {
private:
    shared_ptr<ConditionalExpression> condExpr1;
    shared_ptr<ConditionalExpression> condExpr2;
public:
    OrCondition(
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
