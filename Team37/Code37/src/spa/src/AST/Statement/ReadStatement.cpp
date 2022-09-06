using namespace std;

#include "ReadStatement.h"

ReadStatement::ReadStatement(
        shared_ptr<TNode> parent,
        int lineNum,
        string variableName) : Statement(parent, lineNum), variableName(variableName) { }

string ReadStatement::getVariableName() {
    return this->variableName;
}

void ReadStatement::accept(shared_ptr<ASTVisitor> visitor) {
    visitor->visitReadStatement(shared_from_this());
}
