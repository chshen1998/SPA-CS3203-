#include "PrintStatement.h"

using namespace std;

PrintStatement::PrintStatement(
        shared_ptr<TNode> parent,
        int lineNum,
        string variableName) : Statement(parent, lineNum), variableName(variableName) { }

int PrintStatement::getLineNum() {
    return Statement::getLineNum();
}

string PrintStatement::getVariableName() {
    return this->variableName;
}
