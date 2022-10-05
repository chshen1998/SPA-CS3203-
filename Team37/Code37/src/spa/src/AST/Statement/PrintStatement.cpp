#include "PrintStatement.h"

using namespace std;

PrintStatement::PrintStatement(
        shared_ptr<TNode> parent,
        string variableName) : Statement(parent), variableName(variableName) { }

string PrintStatement::getVariableName() {
    return this->variableName;
}

void PrintStatement::accept(shared_ptr<ASTVisitor> visitor) {
    visitor->visitPrintStatement(shared_from_this());
}

shared_ptr<CFGNode> PrintStatement::buildCFG(vector<shared_ptr<CFGNode>> parents) {
    shared_ptr<CFGNode> cfgNode = make_shared<CFGNode>(shared_from_this(), parents);
    for (auto p: parents) {
        p->addChild(cfgNode);
    }
    return cfgNode;
}