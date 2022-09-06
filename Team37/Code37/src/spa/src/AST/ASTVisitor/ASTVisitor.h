#ifndef SPA_ASTVISITOR_H
#define SPA_ASTVISITOR_H

#include <string>
#include <memory>

#include "AST/TNode.h"

class SourceCode;

class ASTVisitor {
public:
    ASTVisitor() { }
    virtual void visitSourceCode(shared_ptr<SourceCode> sourceCode) = 0;
//    virtual void visitTokenGroup(TokenGroup tokenGroup) = 0;
//    virtual void visitProcedureList(ProcedureList procedureList) = 0;
//    virtual void visitStatementList(StatementList statementList) = 0;
//    virtual void visitToken(Token token) = 0;

};

#endif
