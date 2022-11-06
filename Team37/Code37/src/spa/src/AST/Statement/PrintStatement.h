#pragma once

#ifndef TEAM37_PRINTSTATEMENT_H
#define TEAM37_PRINTSTATEMENT_H

#include "Statement.h"

#include <memory>
#include <string>

class PrintStatement : public Statement, public enable_shared_from_this<PrintStatement> {
private:
    string variableName;

public:
    PrintStatement(shared_ptr<TNode> parent, string variableName);

    /**
     * Gets the name of the variable in the print statement
     *
     * @return name of variable
     */
    string getVariableName();

    void accept(shared_ptr<ASTVisitor> visitor) override;

    shared_ptr<CFGNode> buildCFG(vector<shared_ptr<CFGNode>> parents, shared_ptr<CFG> cfg) override;
};

#endif
