#pragma once

#ifndef TEAM37_WHILESTATEMENT_H
#define TEAM37_WHILESTATEMENT_H

#include <memory>
#include <string>
#include <vector>

#include "../Expression/ConditionalExpression/ConditionalExpression.h"
#include "Statement.h"

class WhileStatement : public Statement, public enable_shared_from_this<WhileStatement> {
private:
    shared_ptr<ConditionalExpression> condExpr;
    vector<shared_ptr<Statement>> stmtLst;

public:
    WhileStatement(shared_ptr<TNode> parent, shared_ptr<ConditionalExpression> condExpr);

    /**
     * Adds a statement to the while statement list
     *
     * @param stmt statement to add
     * @return none
     */
    void addStatement(shared_ptr<Statement> stmt);

    /**
     * Gets the conditional expression in the while statement
     *
     * @return a conditional expression
     */
    shared_ptr<ConditionalExpression> getConditionalExpression();

    /**
     * Gets the statement list
     *
     * @return the statement list
     */
    vector<shared_ptr<Statement>> getStatements();

    void accept(shared_ptr<ASTVisitor> visitor) override;

    shared_ptr<CFGNode> buildCFG(
        vector<shared_ptr<CFGNode>> parents, shared_ptr<CFG> cfg) override;
};

#endif
