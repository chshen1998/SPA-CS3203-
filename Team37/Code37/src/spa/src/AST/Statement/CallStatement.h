#ifndef TEAM37_CALLSTATEMENT_H
#define TEAM37_CALLSTATEMENT_H

#include "Statement.h"

class CallStatement : public Statement {
private:
    string procedureName;
public:
    CallStatement(shared_ptr<TNode> parent, int lineNum, string procedureName);

    int getLineNum() override;

    /**
     * Gets the name of the procedure in the call statement
     *
     * @return name of procedure
     */
    string getProcedureName();
};

#endif
