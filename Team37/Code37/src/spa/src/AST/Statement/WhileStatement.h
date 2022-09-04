#include <vector>

#include "Statement.h"
#include "../ConditionalExpression/ConditionalExpression.h"

class WhileStatement : public Statement {
private:
    ConditionalExpression condExpr;
    vector<shared_ptr<Statement>> stmtLst;
public:
    WhileStatement(shared_ptr<TNode> parent, int lineNum, ConditionalExpression condExpr);

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
    ConditionalExpression getConditionalExpression();

    /**
     * Gets the statement list
     *
     * @return the statement list
     */
    vector<shared_ptr<Statement>> getStatements();

    shared_ptr<TNode> getParent() override;
};
