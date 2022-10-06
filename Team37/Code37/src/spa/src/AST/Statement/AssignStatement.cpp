#include "AssignStatement.h"

using namespace std;

AssignStatement::AssignStatement(
        shared_ptr<TNode> parent,
        string varName,
        shared_ptr<RelationalFactor> relFactor) : Statement(parent), relFactor(relFactor), varName(varName) {}

string AssignStatement::getVarName() {
    return this->varName;
}

shared_ptr<RelationalFactor> AssignStatement::getRelFactor() {
    return this->relFactor;
}

void AssignStatement::accept(shared_ptr<ASTVisitor> visitor) {
    visitor->visitAssignStatement(shared_from_this());
}

shared_ptr<CFGNode> AssignStatement::buildCFG(vector<shared_ptr<CFGNode>> parents, shared_ptr<CFG> cfg) {
    shared_ptr<CFGNode> cfgNode = make_shared<CFGNode>(shared_from_this(), parents);
    for (auto p: parents) {
        p->addChild(cfgNode);
    }

    //store mapping of statement
    cfg->addMapping(this->getLineNum(), cfgNode);

    return cfgNode;
}
