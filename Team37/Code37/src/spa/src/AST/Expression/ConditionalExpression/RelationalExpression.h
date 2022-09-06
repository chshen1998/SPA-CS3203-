#ifndef TEAM37_RELATIONALEXPRESSION_H
#define TEAM37_RELATIONALEXPRESSION_H

#include "ConditionalExpression.h"
#include "AST/Operators/RelationalOperator.h"
#include "../RelationalFactor/RelationalFactor.h"


class RelationalExpression : public ConditionalExpression {
private:
    RelationalOperator opr;
    shared_ptr<RelationalFactor> relFactor1;
    shared_ptr<RelationalFactor> relFactor2;
public:
    RelationalExpression(
            shared_ptr<TNode> parent,
            RelationalOperator opr,
            shared_ptr<RelationalFactor> relFactor1,
            shared_ptr<RelationalFactor> relFactor2);

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
    shared_ptr<RelationalFactor> getRelFactor1();

    /**
     * Gets the second relational factor
     *
     * @return an expression (semantically the same as a relational factor)
     */
    shared_ptr<RelationalFactor> getRelFactor2();
};

#endif
