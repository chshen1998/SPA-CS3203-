#ifndef TEAM37_CONSTANTEXPRESSION_H
#define TEAM37_CONSTANTEXPRESSION_H

#include "RelationalFactor.h"

class ConstantExpression : public RelationalFactor {
private:
    int value;
public:
    ConstantExpression(shared_ptr<TNode> parent, int value);

    /**
     * Get the value in the constant expression
     *
     * @return value of the constant expression
     */
    int getValue();
};

#endif
