#ifndef TEAM37_NOTCONDITION_H
#define TEAM37_NOTCONDITION_H

#include "ConditionalExpression.h"

class NotCondition : public ConditionalExpression {
private:
    shared_ptr<ConditionalExpression> condExpr;
public:
    NotCondition(shared_ptr<TNode> parent, shared_ptr<ConditionalExpression> condExpr);

    /**
     * Get the conditional expression of a not condition
     *
     * @return conditional expression
     */
    shared_ptr<ConditionalExpression> getConditionalExpression();
};

#endif
