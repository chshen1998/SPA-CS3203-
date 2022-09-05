#include <string>
#include <fstream>
#include <iostream>

using namespace std;

#include "AST/SourceCode.h"

class ParserVisitor: public Visitor {
public:
    // Constructor
    ParserVisitor();
    void visitSourceCode(shared_ptr<SourceCode> sourceCode) override;
//    void visitTokenGroup(TokenGroup tokenGroup);
//    void visitProcedureList(ProcedureList procedureList);
//    void visitStatementList(StatementList statementList);
//    void visitToken(Token token);
};
