#include "CFGNode.h"

CFGNode::CFGNode(shared_ptr<TNode> tNode, vector<shared_ptr<CFGNode>> parents)
    : tNode(tNode)
    , parents(parents)
{
}

vector<shared_ptr<CFGNode>> CFGNode::getParents()
{
    return parents;
}

vector<shared_ptr<CFGNode>> CFGNode::getChildren()
{
    return children;
}

shared_ptr<CFGNode> CFGNode::getChild(int i)
{
    assert(i >= 0 && i < children.size());
    return children[i];
}

void CFGNode::addParent(shared_ptr<CFGNode> newParent)
{
    parents.push_back(newParent);
}

void CFGNode::addChild(shared_ptr<CFGNode> newChild)
{
    children.push_back(newChild);
}

int CFGNode::getNumChildren()
{
    return children.size();
}

shared_ptr<TNode> CFGNode::getTNode()
{
    return tNode;
}

void CFGNode::setStmtToStore(shared_ptr<CFGNode> stmt)
{
    stmtToStore = stmt;
}

shared_ptr<CFGNode> CFGNode::getStoredStmt()
{
    return stmtToStore;
}