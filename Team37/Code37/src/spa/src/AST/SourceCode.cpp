#include "SourceCode.h"

SourceCode::SourceCode(string filename) : TNode(nullptr), filename(filename) {}

void SourceCode::addProcedure(shared_ptr<Procedure> procedure) {
    this->procedureLst.push_back(procedure);
}

vector<shared_ptr<Procedure> > SourceCode::getProcedures() {
    return this->procedureLst;
}

string SourceCode::getFileName() {
    return this->filename;
}

void SourceCode::setNumOfStatements(int num) {
    numOfStatements = num;
}

int SourceCode::getNumOfStatements() const {
    return numOfStatements;
}

void SourceCode::accept(shared_ptr<ASTVisitor> visitor) {
    visitor->visitSourceCode(shared_from_this());
}

vector<shared_ptr<CFG> > SourceCode::getAllCFGs() {
    vector<shared_ptr<CFG>> CFGLst;
    for (auto p: this->procedureLst) {
        p->buildCFG(p->getProcedureName());
        CFGLst.push_back(p->getCFG());
    }
    return CFGLst;
}

shared_ptr<map<int, shared_ptr<CFGNode>>> SourceCode::getAllCFGMaps() {
    shared_ptr<map<int, shared_ptr<CFGNode>>> allMaps = make_shared<map<int, shared_ptr<CFGNode>>>();
    for (auto p: this->procedureLst) {
        shared_ptr<map<int, shared_ptr<CFGNode>>> mapInProcedure = p->getCFG()->getMap();
        allMaps->insert(mapInProcedure->begin(), mapInProcedure->end());
    }
    return allMaps;
}

shared_ptr<AllCFGs> SourceCode::getAllCFGInfo() {
    vector<shared_ptr<CFG> > allCFGs = this->getAllCFGs();
    shared_ptr<map<int, shared_ptr<CFGNode> > > combinedMap = this->getAllCFGMaps();
    shared_ptr<AllCFGs> allCFGsInfo = make_shared<AllCFGs>(allCFGs, combinedMap);
    return allCFGsInfo;
}