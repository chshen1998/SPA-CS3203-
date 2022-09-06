#include "CallStatement.h"

using namespace std;

CallStatement::CallStatement(
        shared_ptr<TNode> parent,
        int lineNum,
        string procedureName)
        : Statement(parent, lineNum), procedureName(procedureName) { }

string CallStatement::getProcedureName() {
    return this->procedureName;
}

void CallStatement::accept(shared_ptr<ASTVisitor> visitor) {
    visitor->visitCallStatement(shared_from_this());
}
