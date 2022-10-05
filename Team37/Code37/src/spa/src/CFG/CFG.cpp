#include "CFG.h"

CFG::CFG(shared_ptr<CFGNode> startNode, string procName) : startNode(startNode), procName(procName) {}

string CFG::getName() {
    return procName;
}

shared_ptr<CFGNode> CFG::getStartNode() {
    return startNode;
}

void CFG::setStartNode(shared_ptr<CFGNode> newStartNode) {
    startNode = newStartNode;
}