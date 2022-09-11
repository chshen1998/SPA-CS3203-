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

#include "PKB/Storage.h"

ExtractASTVisitor::ExtractASTVisitor(shared_ptr<Storage> storage) { this->storage = storage; }

vector<shared_ptr<NameExpression>> ExtractASTVisitor::getTNodeVariables() {
    return VisitedTNodeVariables;
};

vector<shared_ptr<ConstantExpression>> ExtractASTVisitor::getVisitedConstants() {
    return VisitedConstants;
}

/**
 * We traverse all procedures in the source code to accept a visitor
 * @param sourceCode
 */
void ExtractASTVisitor::visitSourceCode(shared_ptr<SourceCode> sourceCode) {
    vector<shared_ptr<Procedure>> procedures = sourceCode->getProcedures();
    for (auto procedure: procedures) {
        procedure->accept(shared_from_this());
    }
}

/**
 * We traverse all statements in the procedure to accept a visitor
 * @param procedure
 */
void ExtractASTVisitor::visitProcedure(shared_ptr<Procedure> procedure) {
    vector<shared_ptr<Statement>> statements = procedure->getStatements();
    for (auto statement: statements) {
        statement->accept(shared_from_this());
    }

}

// Statements

/**
 * We visit a read statement and store it as a NameExpression as it contains a variable
 * @param readStmt
 */
void ExtractASTVisitor::visitReadStatement(shared_ptr<ReadStatement> readStmt) {
    shared_ptr<NameExpression> expression = make_shared<NameExpression>(readStmt, readStmt->getVariableName());
    this->storage->storeVar(expression);
//    (this->VisitedTNodeVariables).push_back(expression);
}

/**
 * We visit a print statement and store it as a NameExpression as it contains a variable
 * @param printStmt
 */
void ExtractASTVisitor::visitPrintStatement(shared_ptr<PrintStatement> printStmt) {
    shared_ptr<NameExpression> expression = make_shared<NameExpression>(printStmt, printStmt->getVariableName());
    (this->VisitedTNodeVariables).push_back(expression);
}

/**
 *
 * @param callStmt
 */
void ExtractASTVisitor::visitCallStatement(shared_ptr<CallStatement> callStmt) {

}

/**
 * We traverse all the statements in a while loop to accept a visitor
 * @param whileStmt
 */
void ExtractASTVisitor::visitWhileStatement(shared_ptr<WhileStatement> whileStmt) {
    vector<shared_ptr<Statement>> statements = whileStmt->getStatements();
    for (auto statement: statements) {
        statement->accept(shared_ptr<ASTVisitor>(this));
    }
}

/**
 * We traverse all the statements in if-else to accept a visitor
 * @param ifStmt
 */
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

/**
 * We visit an assign statement and store it as a NameExpression as it contains a variable
 * @param assignStmt
 */
void ExtractASTVisitor::visitAssignStatement(shared_ptr<AssignStatement> assignStmt) {
    shared_ptr<NameExpression> expression = make_shared<NameExpression>(assignStmt, assignStmt->getVarName());
    (this->VisitedTNodeVariables).push_back(expression);
}

// RelationalFactor

void ExtractASTVisitor::visitNameExpression(shared_ptr<NameExpression> nameExpr) {

}

/**
 * We visit a constant expression and store it as a constant
 * @param constantExpr
 */
void ExtractASTVisitor::visitConstantExpression(shared_ptr<ConstantExpression> constantExpr) {
    (this->VisitedConstants).push_back(constantExpr);
}

/**
 * We visit an operated expression and traverse both the expression to accept a visitor
 * @param operatedExpr
 */
void ExtractASTVisitor::visitOperatedExpression(shared_ptr<OperatedExpression> operatedExpr) {
    operatedExpr->getExpression1()->accept(shared_ptr<ASTVisitor>(this));
    operatedExpr->getExpression2()->accept(shared_ptr<ASTVisitor>(this));

}

// ConditionalExpression

/**
 * We visit a relational expression and traverse both relational factors to accept a visitor
 * @param relationalExpr
 */
void ExtractASTVisitor::visitRelationalExpression(shared_ptr<RelationalExpression> relationalExpr) {
    relationalExpr->getRelFactor1()->accept(shared_ptr<ASTVisitor>(this));
    relationalExpr->getRelFactor2()->accept(shared_ptr<ASTVisitor>(this));
}

/**
 * We visit a not condition and traverse its conditional expression to accept a visitor
 * @param notCondition
 */
void ExtractASTVisitor::visitNotCondition(shared_ptr<NotCondition> notCondition) {
    notCondition->getConditionalExpression()->accept(shared_ptr<ASTVisitor>(this));
}

/**
 * We visit an And condition and traverse both conditional expression to accept a visitor
 * @param andCondition
 */
void ExtractASTVisitor::visitAndCondition(shared_ptr<AndCondition> andCondition) {
    andCondition->getConditionalExpression1()->accept(shared_ptr<ASTVisitor>(this));
    andCondition->getConditionalExpression2()->accept(shared_ptr<ASTVisitor>(this));
}

/**
 * We visit an Or condition and traverse both conditional expression to accept a visitor
 * @param orCondition
 */
void ExtractASTVisitor::visitOrCondition(shared_ptr<OrCondition> orCondition) {
    orCondition->getConditionalExpression1()->accept(shared_ptr<ASTVisitor>(this));
    orCondition->getConditionalExpression2()->accept(shared_ptr<ASTVisitor>(this));
}



