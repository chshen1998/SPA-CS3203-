#include "CallStatement.h"

using namespace std;

CallStatement::CallStatement(
        shared_ptr<TNode> parent,
        int lineNum,
        string procedureName)
        : Statement(parent, lineNum), procedureName(procedureName) { }

int CallStatement::getLineNum() {
    return Statement::getLineNum();
}

string CallStatement::getProcedureName() {
    return this->procedureName;
}
