#include "Statement.h"

class PrintStatement : public Statement {
private:
    string variableName;

public:
    PrintStatement(shared_ptr<TNode> parent, int lineNum, string variableName);

    int getLineNum() override;

    /**
     * Gets the name of the variable in the print statement
     *
     * @return name of variable
     */
    string getVariableName();

    shared_ptr<TNode> getParent() override;
};
