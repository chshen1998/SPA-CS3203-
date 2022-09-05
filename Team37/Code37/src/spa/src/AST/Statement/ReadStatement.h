using namespace std;

#ifndef TEAM37_READSTATEMENT_H
#define TEAM37_READSTATEMENT_H

#include "Statement.h"

class ReadStatement : public Statement {
private:
    string variableName;

public:
    ReadStatement(shared_ptr<TNode> parent, int lineNum, string variableName);

    int getLineNum();

    /**
     * Gets the name of the variable in the read statement
     *
     * @return name of variable
     */
    string getVariableName();

    shared_ptr<TNode> getParent();
};

#endif //TEAM37_READSTATEMENT_H