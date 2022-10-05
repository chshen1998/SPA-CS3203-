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

void Procedure::buildCFG(shared_ptr<TNode> tNode, string procName) {
    if (this->cfg->getStartNode() == nullptr) {
        this->cfg->setName(procName);
        vector<shared_ptr<CFGNode>> parents = {};
        this->cfg->setStartNode(make_shared<CFGNode>(tNode, parents));
    } else {
        vector<shared_ptr<CFGNode>> parents = this->cfg->getEndNodes();
        vector<shared_ptr<CFGNode>> newEndNodes;
        shared_ptr<CFGNode> newEndNode = make_shared<CFGNode>(tNode, parents);
        newEndNodes.push_back(newEndNode);
        this->cfg->setEndNodes(newEndNodes);
    }
}

void Procedure::accept(shared_ptr<ASTVisitor> visitor) {
    visitor->visitProcedure(shared_from_this());
}
