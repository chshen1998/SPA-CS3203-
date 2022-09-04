#include "ConditionalExpression.h"

class AndCondition : public ConditionalExpression {
private:
    ConditionalExpression condExpr1;
    ConditionalExpression condExpr2;
public:
    AndCondition(ConditionalExpression condExpr1, ConditionalExpression condExpr2);

    /**
     * Get the first conditional expression of a and condition
     *
     * @return conditional expression
     */
    ConditionalExpression getConditionalExpression1();

    /**
     * Get the second conditional expression of a and condition
     *
     * @return conditional expression
     */
    ConditionalExpression getConditionalExpression2();

};
