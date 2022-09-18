#pragma once

#ifndef TEAM37_CALLSTATEMENT_H
#define TEAM37_CALLSTATEMENT_H

#include "Statement.h"

class CallStatement : public Statement, public enable_shared_from_this<CallStatement> {
private:
    string procedureName;
public:
    CallStatement(shared_ptr<TNode> parent, string procedureName);

    /**
     * Gets the name of the procedure in the call statement
     *
     * @return name of procedure
     */
    string getProcedureName();

    void accept(shared_ptr<ASTVisitor> visitor) override;
};

#endif
