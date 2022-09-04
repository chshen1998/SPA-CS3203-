#include "Procedure.h"

using namespace std;

Procedure::Procedure(shared_ptr<TNode> parent, string procedureName) : TNode(parent), procedureName(procedureName) { }

void Procedure::addStatement(shared_ptr<Statement> stmt) {
    this->stmtLst.push_back(stmt);
}

string Procedure::getProcedureName() {
    return this->procedureName;
}

vector<shared_ptr<Statement>> Procedure::getStatements() {
    return this->stmtLst;
}

shared_ptr<TNode> Procedure::getParent() {
    return Procedure::TNode::getParent();
}