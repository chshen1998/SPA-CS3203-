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
    shared_ptr<CFGNode> firstNode = stmtLst[0]->buildCFG(parents);
    shared_ptr<CFGNode> startNode;

    // check if firstNode is a dummy node, as while or if buildCFG returns a dummy node
    if (firstNode->getTNode() == nullptr) {
        // get the stored while or if cfg node
        startNode = firstNode->getStoredStmt();
    } else {
        startNode = firstNode;
    }

    // remove first statement from stmtLst
    stmtLst.erase(stmtLst.begin());

    // reset parents of next node to firstNode
    parents.clear();
    parents.push_back(firstNode);

    for (auto s : stmtLst) {
        shared_ptr<CFGNode> cfgNode = s->buildCFG(parents);
        parents.clear();
        parents.push_back(cfgNode);
    }
    this->cfg = make_shared<CFG>(startNode, procName);
}

void Procedure::accept(shared_ptr<ASTVisitor> visitor) {
    visitor->visitProcedure(shared_from_this());
}
