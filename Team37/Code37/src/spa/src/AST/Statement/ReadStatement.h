#pragma once

#ifndef TEAM37_READSTATEMENT_H
#define TEAM37_READSTATEMENT_H

using namespace std;

#include "Statement.h"

class ReadStatement : public Statement, public enable_shared_from_this<ReadStatement> {
private:
    string variableName;
public:
    ReadStatement(shared_ptr<TNode> parent, int lineNum, string variableName);

    /**
     * Gets the name of the variable in the read statement
     *
     * @return name of variable
     */
    string getVariableName();

    void accept(shared_ptr<ASTVisitor> visitor) override;
};

#endif