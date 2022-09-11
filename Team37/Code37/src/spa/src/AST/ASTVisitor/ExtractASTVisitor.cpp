#include "ExtractASTVisitor.h"

#include "AST/SourceCode.h"
#include "AST/Statement/ReadStatement.h"
#include "AST/Statement/PrintStatement.h"
#include "AST/Statement/CallStatement.h"
#include "AST/Statement/WhileStatement.h"
#include "AST/Statement/IfStatement.h"
#include "AST/Statement/AssignStatement.h"

#include "AST/Expression/RelationalFactor/NameExpression.h"
#include "AST/Expression/RelationalFactor/ConstantExpression.h"
#include "AST/Expression/RelationalFactor/OperatedExpression.h"

#include "AST/Expression/ConditionalExpression/RelationalExpression.h"
#include "AST/Expression/ConditionalExpression/NotCondition.h"
#include "AST/Expression/ConditionalExpression/AndCondition.h"
#include "AST/Expression/ConditionalExpression/OrCondition.h"

ExtractASTVisitor::ExtractASTVisitor() {
}

vector<shared_ptr<NameExpression>> ExtractASTVisitor::getTNodeVariables() {
    return VisitedTNodeVariables;
};

vector<shared_ptr<ConstantExpression>> ExtractASTVisitor::getVisitedConstants() {
    return VisitedConstants;
}


void ExtractASTVisitor::visitSourceCode(shared_ptr<SourceCode> sourceCode) {

}


void ExtractASTVisitor::visitProcedure(shared_ptr<Procedure> procedure) {
    vector<shared_ptr<Statement>> statements = procedure->getStatements();
    for (auto statement: statements) {
        statement->accept(shared_ptr<ASTVisitor>(this));
    }
}

// Statements
void ExtractASTVisitor::visitReadStatement(shared_ptr<ReadStatement> readStmt) {
    shared_ptr<NameExpression> expression = make_shared<NameExpression>(readStmt, readStmt->getVariableName());
    (this->VisitedTNodeVariables).push_back(expression);
}

void ExtractASTVisitor::visitPrintStatement(shared_ptr<PrintStatement> printStmt) {
    shared_ptr<NameExpression> expression = make_shared<NameExpression>(printStmt, printStmt->getVariableName());
    (this->VisitedTNodeVariables).push_back(expression);
}

void ExtractASTVisitor::visitCallStatement(shared_ptr<CallStatement> callStmt) {

}

void ExtractASTVisitor::visitWhileStatement(shared_ptr<WhileStatement> whileStmt) {
    vector<shared_ptr<Statement>> statements = whileStmt->getStatements();
    for (auto statement: statements) {
        statement->accept(shared_ptr<ASTVisitor>(this));
    }
}

void ExtractASTVisitor::visitIfStatement(shared_ptr<IfStatement> ifStmt) {
    vector<shared_ptr<Statement>> thenStmts = ifStmt->getThenStatements();
    for (auto statement: thenStmts) {
        statement->accept(shared_ptr<ASTVisitor>(this));
    }
    vector<shared_ptr<Statement>> elseStmts = ifStmt->getElseStatements();
    for (auto statement: elseStmts) {
        statement->accept(shared_ptr<ASTVisitor>(this));
    }
}

void ExtractASTVisitor::visitAssignStatement(shared_ptr<AssignStatement> assignStmt) {
    shared_ptr<NameExpression> expression = make_shared<NameExpression>(assignStmt, assignStmt->getVarName());
    (this->VisitedTNodeVariables).push_back(expression);
}

// RelationalFactor
void ExtractASTVisitor::visitNameExpression(shared_ptr<NameExpression> nameExpr) {

}

void ExtractASTVisitor::visitConstantExpression(shared_ptr<ConstantExpression> constantExpr) {
    (this->VisitedConstants).push_back(constantExpr);
}

void ExtractASTVisitor::visitOperatedExpression(shared_ptr<OperatedExpression> operatedExpr) {
    operatedExpr->getExpression1()->accept(shared_ptr<ASTVisitor>(this));
    operatedExpr->getExpression2()->accept(shared_ptr<ASTVisitor>(this));

}

// ConditionalExpression
void ExtractASTVisitor::visitRelationalExpression(shared_ptr<RelationalExpression> relationalExpr) {
    relationalExpr->getRelFactor1()->accept(shared_ptr<ASTVisitor>(this));
    relationalExpr->getRelFactor2()->accept(shared_ptr<ASTVisitor>(this));
}

void ExtractASTVisitor::visitNotCondition(shared_ptr<NotCondition> notCondition) {
    notCondition->getConditionalExpression()->accept(shared_ptr<ASTVisitor>(this));
}

void ExtractASTVisitor::visitAndCondition(shared_ptr<AndCondition> andCondition) {
    andCondition->getConditionalExpression1()->accept(shared_ptr<ASTVisitor>(this));
    andCondition->getConditionalExpression2()->accept(shared_ptr<ASTVisitor>(this));
}

void ExtractASTVisitor::visitOrCondition(shared_ptr<OrCondition> orCondition) {
    orCondition->getConditionalExpression1()->accept(shared_ptr<ASTVisitor>(this));
    orCondition->getConditionalExpression2()->accept(shared_ptr<ASTVisitor>(this));
}



