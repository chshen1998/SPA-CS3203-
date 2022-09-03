#include "ConditionalExpression.h"

class NotCondition : public ConditionalExpression {
private:
    ConditionalExpression condExpr;
public:
    NotCondition(ConditionalExpression condExpr);

    /**
     * Get the conditional expression of a not condition
     *
     * @return conditional expression
     */
    ConditionalExpression getConditionalExpression();

};
