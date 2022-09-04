#include "Statement.h"

class ReadStatement : public Statement {
private:
    string variableName;

public:
    ReadStatement(shared_ptr<TNode> parent, int lineNum, string variableName);

    int getLineNum() override;

    /**
     * Gets the name of the variable in the read statement
     *
     * @return name of variable
     */
    string getVariableName();

    shared_ptr<TNode> getParent() override;
};
