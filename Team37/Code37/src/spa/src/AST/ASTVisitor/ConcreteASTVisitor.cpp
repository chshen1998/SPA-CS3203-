#include "ConcreteASTVisitor.h"

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

ConcreteASTVisitor::ConcreteASTVisitor() {
}

vector<shared_ptr<NameExpression>> ConcreteASTVisitor::getTNodeVariables() {
    return TNodeVariables;
};

void ConcreteASTVisitor::visitSourceCode(shared_ptr<SourceCode> sourceCode) {

}


void ConcreteASTVisitor::visitProcedure(shared_ptr<Procedure> procedure) {
    vector<shared_ptr<Statement>> statements = procedure->getStatements();
    for (auto statement: statements) {
        statement->accept(shared_ptr<ASTVisitor>(this));
    }
}

// Statements
void ConcreteASTVisitor::visitReadStatement(shared_ptr<ReadStatement> readStmt) {
    shared_ptr<NameExpression> expression = make_shared<NameExpression>(readStmt, readStmt->getVariableName());
    (this->TNodeVariables).push_back(expression);
}

void ConcreteASTVisitor::visitPrintStatement(shared_ptr<PrintStatement> printStmt) {
    shared_ptr<NameExpression> expression = make_shared<NameExpression>(printStmt, printStmt->getVariableName());
    (this->TNodeVariables).push_back(expression);
}

void ConcreteASTVisitor::visitCallStatement(shared_ptr<CallStatement> callStmt) {

}

void ConcreteASTVisitor::visitWhileStatement(shared_ptr<WhileStatement> whileStmt) {
    vector<shared_ptr<Statement>> statements = whileStmt->getStatements();
    for (auto statement: statements) {
        statement->accept(shared_ptr<ASTVisitor>(this));
    }
}

void ConcreteASTVisitor::visitIfStatement(shared_ptr<IfStatement> ifStmt) {
    vector<shared_ptr<Statement>> thenStmts = ifStmt->getThenStatements();
    for (auto statement: thenStmts) {
        statement->accept(shared_ptr<ASTVisitor>(this));
    }
    vector<shared_ptr<Statement>> elseStmts = ifStmt->getElseStatements();
    for (auto statement: elseStmts) {
        statement->accept(shared_ptr<ASTVisitor>(this));
    }
}

void ConcreteASTVisitor::visitAssignStatement(shared_ptr<AssignStatement> assignStmt) {
    shared_ptr<NameExpression> expression = make_shared<NameExpression>(assignStmt, assignStmt->getVarName());
    (this->TNodeVariables).push_back(expression);
}

// RelationalFactor
void ConcreteASTVisitor::visitNameExpression(shared_ptr<NameExpression> nameExpr) {

}

void ConcreteASTVisitor::visitConstantExpression(shared_ptr<ConstantExpression> constantExpr) {

}

void ConcreteASTVisitor::visitOperatedExpression(shared_ptr<OperatedExpression> operatedExpr) {

}

// ConditionalExpression
void ConcreteASTVisitor::visitRelationalExpression(shared_ptr<RelationalExpression> relationalExpr) {

}

void ConcreteASTVisitor::visitNotCondition(shared_ptr<NotCondition> notCondition) {

}

void ConcreteASTVisitor::visitAndCondition(shared_ptr<AndCondition> andCondition) {

}

void ConcreteASTVisitor::visitOrCondition(shared_ptr<OrCondition> orCondition) {

}



