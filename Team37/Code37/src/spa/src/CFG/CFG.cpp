#include "CFG.h"

CFG::CFG(shared_ptr<CFGNode> startNode) : startNode(startNode) {}

shared_ptr<CFGNode> CFG::getStartNode() {
    return startNode;
}

shared_ptr<CFGNode> CFG::getEndNode() {
    return endNode;
}

void CFG::setEndNode(shared_ptr<CFGNode> newEndNode) {
    endNode = newEndNode;
}