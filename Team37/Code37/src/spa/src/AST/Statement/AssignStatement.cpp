#include "AssignStatement.h"

using namespace std;

AssignStatement::AssignStatement(
        shared_ptr<TNode> parent,
        int lineNum,
        string varName,
        shared_ptr<RelationalFactor> relFactor) : Statement(parent, lineNum), relFactor(relFactor) {}

string AssignStatement::getVarName() {
    return this->varName;
}

shared_ptr<RelationalFactor> AssignStatement::getRelFactor() {
    return this->relFactor;
}

void AssignStatement::accept(shared_ptr<ASTVisitor> visitor) {
    visitor->visitAssignStatement(shared_from_this());
}
