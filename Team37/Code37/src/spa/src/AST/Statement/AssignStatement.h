#ifndef TEAM37_ASSIGNSTATEMENT_H
#define TEAM37_ASSIGNSTATEMENT_H

#include "Statement.h"
#include "../Expression/RelationalFactor/RelationalFactor.h"

class AssignStatement : public Statement {
private:
    string varName;
    shared_ptr<RelationalFactor> relFactor;
public:
    AssignStatement(shared_ptr<TNode> parent, int lineNum, string varName, shared_ptr<RelationalFactor> relFactor);

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
    shared_ptr<RelationalFactor> getRelFactor();
};

#endif
