#include "SourceCode.h"

SourceCode::SourceCode(string filename) : TNode(nullptr) { }

void SourceCode::addProcedure(shared_ptr<Procedure> procedure) {
    this->procedureLst.push_back(procedure);
}

vector<shared_ptr<Procedure> > SourceCode::getProcedures() {
    return this->procedureLst;
}

shared_ptr<TNode> SourceCode::getParent() {
    return SourceCode::TNode::getParent();
}

void SourceCode:: accept(shared_ptr<Visitor> visitor) {
    visitor->visitSourceCode(make_shared<SourceCode>(*this));
}