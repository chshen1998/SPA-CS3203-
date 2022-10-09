#pragma once

#ifndef TEAM37_PROCEDURE_H
#define TEAM37_PROCEDURE_H

#include <vector>
#include <string>
#include <memory>

using namespace std;

#include "./Statement/Statement.h"
#include "CFG/CFG.h"

class Procedure : public TNode, public enable_shared_from_this<Procedure> {
private:
    string procedureName = "";
    vector<shared_ptr<Statement> > stmtLst = {};
//    shared_ptr<CFG> cfg = nullptr;
    shared_ptr<CFG> cfg = make_shared<CFG>(nullptr, "");
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

    /**
     * Gets the CFG associated with this procedure
     * @return the CFG associated with this procedure
     */
    shared_ptr<CFG> getCFG();

    /**
     * Builds the CFG for the procedure
     * @param procName Name of the procedure
     */
    void buildCFG(string procName);

    void accept(shared_ptr<ASTVisitor> visitor) override;

    bool operator==(const Procedure& other) const {
        return procedureName == other.procedureName && stmtLst == other.stmtLst;
    };

    bool operator<(const Procedure& other) const {
        return procedureName > other.procedureName;
    };
};

#endif //TEAM37_PROCEDURE_H