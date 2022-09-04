#include "Expression.h"
#include "AST/Operators/Operator.h"

class OperatedExpression : public Expression {
private:
    Operator opr;
    Expression expr1;
    Expression expr2;
public:
    OperatedExpression(Operator opr, Expression expr1, Expression expr2);

    /**
     * Get the operator of the operated expression
     *
     * @return an operator
     */
    Operator getOperator();

    /**
     * Get the first expression of the operated expression
     *
     * @return an expression
     */
    Expression getExpression1();

    /**
     * Get the second expression of the operated expression
     *
     * @return an expression
     */
    Expression getExpression2();

};
