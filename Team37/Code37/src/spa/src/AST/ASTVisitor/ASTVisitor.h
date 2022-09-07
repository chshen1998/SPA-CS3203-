#pragma once

#ifndef TEAM37_ASTVISITOR_H
#define TEAM37_ASTVISITOR_H

//#include "AST/SourceCode.h"
//
//#include "AST/Statement/ReadStatement.h"
//#include "AST/Statement/PrintStatement.h"
//#include "AST/Statement/CallStatement.h"
//#include "AST/Statement/WhileStatement.h"
//#include "AST/Statement/IfStatement.h"
//#include "AST/Statement/AssignStatement.h"
//
//#include "AST/Expression/RelationalFactor/NameExpression.h"
//#include "AST/Expression/RelationalFactor/ConstantExpression.h"
//#include "AST/Expression/RelationalFactor/OperatedExpression.h"
//
//#include "AST/Expression/ConditionalExpression/RelationalExpression.h"
//#include "AST/Expression/ConditionalExpression/NotCondition.h"
//#include "AST/Expression/ConditionalExpression/AndCondition.h"
//#include "AST/Expression/ConditionalExpression/OrCondition.h"

class SourceCode;
class Procedure;
class ReadStatement;
class PrintStatement;
class CallStatement;
class WhileStatement;
class IfStatement;
class AssignStatement;
class NameExpression;
class ConstantExpression;
class OperatedExpression;
class RelationalExpression;
class NotCondition;
class AndCondition;
class OrCondition;

class ASTVisitor {
public:
    virtual void visitSourceCode(shared_ptr<SourceCode> sourceCode) = 0;
    virtual void visitProcedure(shared_ptr<Procedure> procedure) = 0;

    // Statements
    virtual void visitReadStatement(shared_ptr<ReadStatement> readStmt) = 0;
    virtual void visitPrintStatement(shared_ptr<PrintStatement> printStmt) = 0;
    virtual void visitCallStatement(shared_ptr<CallStatement> callStmt) = 0;
    virtual void visitWhileStatement(shared_ptr<WhileStatement> whileStmt) = 0;
    virtual void visitIfStatement(shared_ptr<IfStatement> ifStmt) = 0;
    virtual void visitAssignStatement(shared_ptr<AssignStatement> assignStmt) = 0;

    // RelationalFactor
    virtual void visitNameExpression(shared_ptr<NameExpression> nameExpr) = 0;
    virtual void visitConstantExpression(shared_ptr<ConstantExpression> constantExpr) = 0;
    virtual void visitOperatedExpression(shared_ptr<OperatedExpression> operatedExpr) = 0;

    // ConditionalExpression
    virtual void visitRelationalExpression(shared_ptr<RelationalExpression> relationalExpr) = 0;
    virtual void visitNotCondition(shared_ptr<NotCondition> notCondition) = 0;
    virtual void visitAndCondition(shared_ptr<AndCondition> andCondition) = 0;
    virtual void visitOrCondition(shared_ptr<OrCondition> orCondition) = 0;
};

#endif
