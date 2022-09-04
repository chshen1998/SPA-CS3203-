#include <string>

using namespace std;

class SourceCode;

class Visitor {
public:
    Visitor();
    virtual void visitSourceCode(shared_ptr<SourceCode> sourceCode) = 0;
//    virtual void visitTokenGroup(TokenGroup tokenGroup) = 0;
//    virtual void visitProcedureList(ProcedureList procedureList) = 0;
//    virtual void visitStatementList(StatementList statementList) = 0;
//    virtual void visitToken(Token token) = 0;

};
