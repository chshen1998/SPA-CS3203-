#include "CFGNode.h"

CFGNode::CFGNode(shared_ptr<TNode> tNode, shared_ptr<CFGNode> parent, vector<shared_ptr<CFGNode>> children)
        : tNode(tNode), parent(parent), children(children) {}

shared_ptr<CFGNode> CFGNode::getParent() {
    return parent;
}

vector<shared_ptr<CFGNode>> CFGNode::getChildren() {
    return children;
}

shared_ptr<CFGNode> CFGNode::getChild(int i) {
    assert(i >= 0 && i < children.size());
    return children[i];
}

void CFGNode::setParent(shared_ptr<CFGNode> newParent) {
    parent = newParent;
}

void CFGNode::addChild(shared_ptr<CFGNode> newChild) {
    children.push_back(newChild);
}

int CFGNode::getNumChildren() {
    return children.size();
}

shared_ptr<TNode> CFGNode::getTNode() {
    return tNode;
}

void CFGNode::setAsVisited() {
    visited = 1;
}