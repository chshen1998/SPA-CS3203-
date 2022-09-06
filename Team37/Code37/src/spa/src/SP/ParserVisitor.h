using namespace std;

#ifndef TEAM37_PARSERVISITOR_H
#define TEAM37_PARSERVISITOR_H

#include <string>
#include <fstream>
#include <iostream>

#include "AST/SourceCode.h"

class ParserVisitor: public Visitor {
public:
    // Constructor
    ParserVisitor();
    shared_ptr<TNode> visitSourceCode(SourceCode* sourceCode);
//    void visitTokenGroup(TokenGroup tokenGroup);
//    void visitProcedureList(ProcedureList procedureList);
//    void visitStatementList(StatementList statementList);
//    void visitToken(Token token);
};
#endif