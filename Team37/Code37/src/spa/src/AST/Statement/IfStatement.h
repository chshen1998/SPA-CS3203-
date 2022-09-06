#ifndef TEAM37_IFSTATEMENT_H
#define TEAM37_IFSTATEMENT_H

#include <vector>

#include "Statement.h"
#include "../Expression/ConditionalExpression/ConditionalExpression.h"

class IfStatement : public Statement {
private:
    shared_ptr<ConditionalExpression> condExpr;
    vector<shared_ptr<Statement> > thenStmtLst;
    vector<shared_ptr<Statement> > elseStmtLst;
public:
    IfStatement(shared_ptr<TNode> parent, int lineNum, shared_ptr<ConditionalExpression> condExpr);

    /**
     * Adds a statement to the if-then statement list
     *
     * @param stmt statement to add
     * @return none
     */
    void addThenStatement(shared_ptr<Statement> stmt);

    /**
     * Adds a statement to the if-else statement list
     *
     * @param stmt statement to add
     * @return none
     */
    void addElseStatement(shared_ptr<Statement> stmt);

    /**
     * Gets the conditional expression in the if statement
     *
     * @return a conditional expression
     */
    shared_ptr<ConditionalExpression> getConditionalExpression();

    /**
     * Gets the if-then statement list
     *
     * @return the if-then statement list
     */
    vector<shared_ptr<Statement> > getThenStatements();

    /**
     * Gets the if-else statement list
     *
     * @return the if-else statement list
     */
    vector<shared_ptr<Statement> > getElseStatements();
};

#endif
