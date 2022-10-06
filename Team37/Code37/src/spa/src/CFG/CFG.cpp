#include "CFG.h"

CFG::CFG(shared_ptr<CFGNode> startNode, string procName) : startNode(startNode), procName(procName) {}

string CFG::getName() {
    return procName;
}

void CFG::setName(std::string name) {
    procName = name;
}

shared_ptr<CFGNode> CFG::getStartNode() {
    return startNode;
}

void CFG::setStartNode(shared_ptr<CFGNode> newStartNode) {
    startNode = newStartNode;
}

void CFG::addMapping(int stmtNum, shared_ptr <CFGNode> cfgNode) {
    stmtCFGNodeMap->insert(std::pair<int, shared_ptr<CFGNode> >(stmtNum, cfgNode));
}

shared_ptr<CFGNode> CFG::getCFGNode(int stmtNum) {
    return stmtCFGNodeMap->at(stmtNum);
}

shared_ptr<map<int, shared_ptr<CFGNode> > > CFG::getMap() {
    return stmtCFGNodeMap;
}