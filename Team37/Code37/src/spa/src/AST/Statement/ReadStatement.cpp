using namespace std;

#include "ReadStatement.h"

ReadStatement::ReadStatement(
    shared_ptr<TNode> parent,
    string variableName)
    : Statement(parent)
    , variableName(variableName)
{
}

string ReadStatement::getVariableName()
{
    return this->variableName;
}

void ReadStatement::accept(shared_ptr<ASTVisitor> visitor)
{
    visitor->visitReadStatement(shared_from_this());
}

shared_ptr<CFGNode> ReadStatement::buildCFG(vector<shared_ptr<CFGNode>> parents, shared_ptr<CFG> cfg)
{
    shared_ptr<CFGNode> cfgNode = make_shared<CFGNode>(shared_from_this(), parents);
    for (auto p : parents) {
        p->addChild(cfgNode);
    }

    // store mapping of statement
    cfg->addMapping(this->getLineNum(), cfgNode);

    return cfgNode;
}