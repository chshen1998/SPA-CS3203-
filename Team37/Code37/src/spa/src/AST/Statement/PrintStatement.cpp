#include "PrintStatement.h"

using namespace std;

PrintStatement::PrintStatement(
        shared_ptr<TNode> parent,
        string variableName) : Statement(parent), variableName(variableName) { }

string PrintStatement::getVariableName() {
    return this->variableName;
}

void PrintStatement::accept(shared_ptr<ASTVisitor> visitor) {
    visitor->visitPrintStatement(shared_from_this());
}