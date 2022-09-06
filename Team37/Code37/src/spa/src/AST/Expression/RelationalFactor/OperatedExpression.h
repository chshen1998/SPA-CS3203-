#pragma once

#ifndef TEAM37_OPERATEDEXPRESSION_H
#define TEAM37_OPERATEDEXPRESSION_H

#include "RelationalFactor.h"
#include "AST/Operators/Operator.h"

class OperatedExpression : public RelationalFactor, public enable_shared_from_this<OperatedExpression> {
private:
    Operator opr;
    shared_ptr<RelationalFactor> relFactor1;
    shared_ptr<RelationalFactor> relFactor2;
public:
    OperatedExpression(
            shared_ptr<TNode> parent,
            Operator opr,
            shared_ptr<RelationalFactor> expr1,
            shared_ptr<RelationalFactor> expr2);

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
    shared_ptr<RelationalFactor> getExpression1();

    /**
     * Get the second expression of the operated expression
     *
     * @return an expression
     */
    shared_ptr<RelationalFactor> getExpression2();

    void accept(shared_ptr<ASTVisitor> visitor) override;
};

#endif
