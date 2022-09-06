#ifndef TEAM37_PRINTSTATEMENT_H
#define TEAM37_PRINTSTATEMENT_H

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
};

#endif
