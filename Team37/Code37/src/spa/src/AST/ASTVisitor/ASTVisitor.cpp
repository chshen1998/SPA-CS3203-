#include "ASTVisitor.h"

using namespace std;

#include "AST/SourceCode.h"
#include "AST/TNode.h"

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

ASTVisitor::ASTVisitor(vector<shared_ptr<TNode>> TNodeVariables) {}

void visitSourceCode() {
}


void ASTVisitor::visitProcedure(shared_ptr<Procedure> procedure) {
    vector<shared_ptr<Statement>> statements = procedure->getStatements();
    for (auto statement: statements) {
        statement->accept(shared_ptr<ASTVisitor> (this));
    }
}

// Statements
void ASTVisitor::visitReadStatement(shared_ptr<ReadStatement> readStmt) {
}

void ASTVisitor::visitPrintStatement(shared_ptr<PrintStatement> printStmt) {
    (this->TNodeVariables).push_back(printStmt);
}

void ASTVisitor::visitCallStatement(shared_ptr<CallStatement> callStmt) {

}

void ASTVisitor::visitWhileStatement(shared_ptr<WhileStatement> whileStmt) {
}

void ASTVisitor::visitIfStatement(shared_ptr<IfStatement> ifStmt) {

}

void ASTVisitor::visitAssignStatement(shared_ptr<AssignStatement> assignStmt) {

}

// RelationalFactor
void ASTVisitor::visitNameExpression(shared_ptr<NameExpression> nameExpr) {

}

void ASTVisitor::visitConstantExpression(shared_ptr<ConstantExpression> constantExpr) {

}

void ASTVisitor::visitOperatedExpression(shared_ptr<OperatedExpression> operatedExpr) {

}

// ConditionalExpression
void ASTVisitor::visitRelationalExpression(shared_ptr<RelationalExpression> relationalExpr) {

}

void ASTVisitor::visitNotCondition(shared_ptr<NotCondition> notCondition) {

}

void ASTVisitor::visitAndCondition(shared_ptr<AndCondition> andCondition) {

}

void ASTVisitor::visitOrCondition(shared_ptr<OrCondition> orCondition) {

}

vector<shared_ptr<TNode>> ASTVisitor::getTNodeVariables() {
    return this->TNodeVariables;
}

