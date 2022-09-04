#include "Expression.h"

class ConstantExpression : public Expression {
private:
    int value;
public:
    ConstantExpression(int value);

    /**
     * Get the value in the constant expression
     *
     * @return value of the constant expression
     */
    int getValue();
};
