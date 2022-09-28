#include "CFG.h"

CFG::CFG(shared_ptr<CFGNode> startNode) : startNode(startNode) {}

shared_ptr<CFGNode> CFG::getStartNode() {
    return startNode;
}

vector<shared_ptr<CFGNode>> CFG::getEndNodes() {
    return endNodes;
}

shared_ptr<CFGNode> CFG::getEndNode(int i) {
    assert(i >= 0 && i < endNodes.size());
    return endNodes[i];
}

void CFG::setEndNode(shared_ptr<CFGNode> newEndNode) {
    endNodes.push_back(newEndNode);
}