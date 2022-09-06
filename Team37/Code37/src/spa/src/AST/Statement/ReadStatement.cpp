#include "ReadStatement.h"

using namespace std;

ReadStatement::ReadStatement(
        shared_ptr<TNode> parent,
        int lineNum,
        string variableName) : Statement(parent, lineNum), variableName(variableName) { }

int ReadStatement::getLineNum() {
    return Statement::getLineNum();
}

string ReadStatement::getVariableName() {
    return this->variableName;
}
