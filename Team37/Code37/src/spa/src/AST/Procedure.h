#pragma once

#ifndef TEAM37_PROCEDURE_H
#define TEAM37_PROCEDURE_H

#include <vector>
#include <string>

using namespace std;

#include "./Statement/Statement.h"

class Procedure : public TNode, public enable_shared_from_this<Procedure> {
private:
    string procedureName = "";
    vector<shared_ptr<Statement> > stmtLst = {};
public:
    Procedure(shared_ptr<TNode> parent, string procedureName);

    /**
     * Adds a statement to the statement list
     *
     * @param stmt the statement to add to the list of the procedure
     * @return none
     */
    void addStatement(shared_ptr<Statement> stmt);

    /**
     * Get the procedure name
     *
     * @return the procedure name
     */
    string getProcedureName();

    /**
     * Gets the statement list
     *
     * @return the list of statements
     */
    vector<shared_ptr<Statement> > getStatements();

    void accept(shared_ptr<ASTVisitor> visitor) override;
};

#endif //TEAM37_PROCEDURE_H