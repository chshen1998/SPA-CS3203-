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

void SourceCode::accept(shared_ptr<CFGVisitor> visitor) {
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

vector<shared_ptr<map<int, shared_ptr<CFGNode> > > > SourceCode::getAllCFGMaps() {
    vector<shared_ptr<map<int, shared_ptr<CFGNode> > > > allMaps;
    for (auto p: this->procedureLst) {
        allMaps.push_back(p->getCFG()->getMap());
    }
    return allMaps;
}