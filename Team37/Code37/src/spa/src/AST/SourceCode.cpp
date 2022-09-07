#include <vector>
#include <string>

#include "SourceCode.h"
#include "Procedure.h"

SourceCode::SourceCode(string filename) : TNode(nullptr), filename(filename) { }

void SourceCode::addProcedure(shared_ptr<Procedure> procedure) {
    this->procedureLst.push_back(procedure);
}

vector<shared_ptr<Procedure> > SourceCode::getProcedures() {
    return this->procedureLst;
}

string SourceCode::getFileName() {
    return this->filename;
}

void SourceCode::accept(shared_ptr<ASTVisitor> visitor) {
    visitor->visitSourceCode(shared_from_this()); //TODO parservisitor takes in *sourcecode
}
