#include "CallStatement.h"

using namespace std;

CallStatement::CallStatement(
        shared_ptr<TNode> parent,
        string procedureName)
        : Statement(parent), procedureName(procedureName) { }

string CallStatement::getProcedureName() {
    return this->procedureName;
}

void CallStatement::accept(shared_ptr<ASTVisitor> visitor) {
    visitor->visitCallStatement(shared_from_this());
}

shared_ptr<CFGNode> CallStatement::buildCFG(vector<shared_ptr<CFGNode>> parents, shared_ptr<CFG> cfg) {
    shared_ptr<CFGNode> cfgNode = make_shared<CFGNode>(shared_from_this(), parents);
    for (auto p: parents) {
        p->addChild(cfgNode);
    }

    //store mapping of statement
    cfg->addMapping(this->getLineNum(), cfgNode);

    return cfgNode;
}