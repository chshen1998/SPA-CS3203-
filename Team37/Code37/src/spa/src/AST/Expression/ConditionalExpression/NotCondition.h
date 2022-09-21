#pragma once

#ifndef TEAM37_NOTCONDITION_H
#define TEAM37_NOTCONDITION_H

#include "ConditionalExpression.h"

#include <string>
#include <memory>

class NotCondition : public ConditionalExpression, public enable_shared_from_this<NotCondition> {
private:
    shared_ptr<ConditionalExpression> condExpr = nullptr;
public:
    NotCondition(shared_ptr<TNode> parent, shared_ptr<ConditionalExpression> condExpr);

    /**
     * Get the conditional expression of a not condition
     *
     * @return conditional expression
     */
    shared_ptr<ConditionalExpression> getConditionalExpression();

    void accept(shared_ptr<ASTVisitor> visitor) override;
};

#endif
