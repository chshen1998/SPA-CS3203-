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

shared_ptr<CFG> Procedure::getCFG() {
    return this->cfg;
}

void Procedure::buildCFG(string procName) {
    // start node does not have any parents
    vector<shared_ptr<CFGNode> > parents = {};

    // set first statement as start node of CFG
    vector<shared_ptr<Statement> > stmtLst = this->getStatements();
    shared_ptr<CFGNode> startNode = make_shared<CFGNode>(stmtLst[0], parents);

    // remove first statement from stmtLst
    stmtLst.erase(stmtLst.begin());

    // reset parents of next node to startNode
    parents.clear();
    parents.push_back(startNode);

    for (auto s: stmtLst) { //todo statement list is all statements irregardless of nesting level???
        shared_ptr<CFGNode> cfgNode = s->buildCFG(parents);
        parents.clear();
        parents.push_back(cfgNode);
    }
    this->cfg = make_shared<CFG>(startNode, procName);
}

void Procedure::accept(shared_ptr<ASTVisitor> visitor) {
    visitor->visitProcedure(shared_from_this());
}
