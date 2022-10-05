#include "CFG.h"

CFG::CFG(shared_ptr<CFGNode> startNode) : startNode(startNode) {}

void CFG::setName(string name) {
    procName = name;
}

shared_ptr<CFGNode> CFG::getStartNode() {
    return startNode;
}

void CFG::setStartNode(shared_ptr<CFGNode> newStartNode) {
    startNode = newStartNode;
}

vector<shared_ptr<CFGNode>> CFG::getEndNodes() {
    return endNodes;
}

shared_ptr<CFGNode> CFG::getEndNode(int i) {
    assert(i >= 0 && i < endNodes.size());
    return endNodes[i];
}

void CFG::addEndNode(shared_ptr<CFGNode> newEndNode) {
    endNodes.push_back(newEndNode);
}

void CFG::setEndNodes(vector<shared_ptr<CFGNode>> newEndNodes) {
    endNodes.clear();
    endNodes = newEndNodes;
}