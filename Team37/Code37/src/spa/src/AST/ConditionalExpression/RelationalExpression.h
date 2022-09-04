#include "ConditionalExpression.h"
#include "../Operators/RelationalOperator.h"


class RelationalExpression : public ConditionalExpression {
private:
    RelationalOperator opr;
    Expression relFactor1;
    Expression relFactor2;
public:
    RelationalExpression(RelationalOperator opr, Expression relFactor1, Expression relFactor2);

    /**
     * Get the relational operator for the conditional expression
     *
     * @return a relational operator
     */
    RelationalOperator getOperator();

    /**
     * Gets the first relational factor
     *
     * @return an expression (semantically the same as a relational factor)
     */
    Expression getRelFactor1();

    /**
     * Gets the second relational factor
     *
     * @return an expression (semantically the same as a relational factor)
     */
    Expression getRelFactor2();

};
