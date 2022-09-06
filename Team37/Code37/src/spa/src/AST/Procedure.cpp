#include "Procedure.h"

Procedure::Procedure(shared_ptr<TNode> parent, string procedureName) : TNode(parent), procedureName(procedureName) { }

void Procedure::addStatement(shared_ptr<Statement> stmt) {
    this->stmtLst.push_back(stmt);
}

string Procedure::getProcedureName() {
    return this->procedureName;
}

vector<shared_ptr<Statement> > Procedure::getStatements() {
    return this->stmtLst;
}

void Procedure::accept(shared_ptr<ASTVisitor> visitor) {
    visitor->visitProcedure(shared_from_this());
}
