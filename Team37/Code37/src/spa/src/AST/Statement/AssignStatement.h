#include "Statement.h"
#include "../Expression/Expression.h"

class AssignStatement : public Statement {
private:
    string varName;
    Expression expr;
public:
    AssignStatement(shared_ptr<TNode> parent, int lineNum, string varName, Expression expr);

    /**
     * Get the variable name of the assign statement
     *
     * @return variable name
     */
    string getVarName();

    /**
     * Get the expression of the assign statement
     *
     * @return expression
     */
    Expression getExpression();

    shared_ptr<TNode> getParent() override;
};
