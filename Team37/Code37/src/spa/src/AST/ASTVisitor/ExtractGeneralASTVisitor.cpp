#include "ExtractGeneralASTVisitor.h"

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

ExtractGeneralASTVisitor::ExtractGeneralASTVisitor(shared_ptr<Storage> storage) { this->storage = storage; }

/**
 * We traverse all procedures in the source code to accept a visitor
 * @param sourceCode
 */
void ExtractGeneralASTVisitor::visitSourceCode(shared_ptr<SourceCode> sourceCode) {

    vector<shared_ptr<Procedure>> procedures = sourceCode->getProcedures();

    for (auto procedure: procedures) {
        procedure->accept(shared_from_this());
    }
}

/**
 * We traverse all statements in the procedure to accept a visitor
 * @param procedure
 */
void ExtractGeneralASTVisitor::visitProcedure(shared_ptr<Procedure> procedure) {
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
void ExtractGeneralASTVisitor::visitReadStatement(shared_ptr<ReadStatement> readStmt) {
    NameExpression expression = NameExpression(readStmt->getParent(),
                                               readStmt->getVariableName());
    // store itself in statements
    this->storage->storeStmt(readStmt);
    // store the variable
    this->storage->storeVar(expression);
}

/**
 * We visit a print statement and store it as a NameExpression as it contains a variable
 * @param printStmt
 */
void ExtractGeneralASTVisitor::visitPrintStatement(shared_ptr<PrintStatement> printStmt) {
    NameExpression expression = NameExpression(printStmt->getParent(),
                                               printStmt->getVariableName());
    // store itself in statements
    this->storage->storeStmt(printStmt);
    // store the variable
    this->storage->storeVar(expression);
}

/**
 *
 * @param callStmt
 */
void ExtractGeneralASTVisitor::visitCallStatement(shared_ptr<CallStatement> callStmt) {
    // store itself in statements
    this->storage->storeStmt(callStmt);
}

/**
 * We traverse all the statements in a while loop to accept a visitor
 * @param whileStmt
 */
void ExtractGeneralASTVisitor::visitWhileStatement(shared_ptr<WhileStatement> whileStmt) {
    // iterate into  statements
    vector<shared_ptr<Statement>> statements = whileStmt->getStatements();

    for (auto statement: statements) {
        statement->accept(shared_from_this());
    }

    // iterate into conditional expressions
    whileStmt->getConditionalExpression()->accept(shared_from_this());

    // store itself in statements
    this->storage->storeStmt(whileStmt);
}

/**
 * We traverse all the statements in if-else to accept a visitor
 * @param ifStmt
 */
void ExtractGeneralASTVisitor::visitIfStatement(shared_ptr<IfStatement> ifStmt) {
    // iterate into statements
    vector<shared_ptr<Statement>> thenStmts = ifStmt->getThenStatements();
    for (auto statement: thenStmts) {
        statement->accept(shared_from_this());
    }
    vector<shared_ptr<Statement>> elseStmts = ifStmt->getElseStatements();
    for (auto statement: elseStmts) {
        statement->accept(shared_from_this());
    }

    // store itself in statements
    this->storage->storeStmt(ifStmt);
}

/**
 * We visit an assign statement and store it as a NameExpression as it contains a variable
 * @param assignStmt
 */
void ExtractGeneralASTVisitor::visitAssignStatement(shared_ptr<AssignStatement> assignStmt) {
    NameExpression expression = NameExpression(assignStmt->getParent(),
                                               assignStmt->getVarName());
    // store itself in statements
    this->storage->storeStmt(assignStmt);
    // store the variable
    this->storage->storeVar(expression);
}

// RelationalFactor

void ExtractGeneralASTVisitor::visitNameExpression(shared_ptr<NameExpression> nameExpr) {
}

/**
 * We visit a constant expression and store it as a constant
 * @param constantExpr
 */
void ExtractGeneralASTVisitor::visitConstantExpression(shared_ptr<ConstantExpression> constantExpr) {
    // store itself in constants
    this->storage->storeConst(*constantExpr.get());
}

/**
 * We visit an operated expression and traverse both the expression to accept a visitor
 * @param operatedExpr
 */
void ExtractGeneralASTVisitor::visitOperatedExpression(shared_ptr<OperatedExpression> operatedExpr) {
    // iterate into expressions
    operatedExpr->getExpression1()->accept(shared_from_this());
    operatedExpr->getExpression2()->accept(shared_from_this());
}

// ConditionalExpression

/**
 * We visit a relational expression and traverse both relational factors to accept a visitor
 * @param relationalExpr
 */
void ExtractGeneralASTVisitor::visitRelationalExpression(shared_ptr<RelationalExpression> relationalExpr) {
    // iterate into expressions
    relationalExpr->getRelFactor1()->accept(shared_from_this());
    relationalExpr->getRelFactor2()->accept(shared_from_this());
}

/**
 * We visit a not condition and traverse its conditional expression to accept a visitor
 * @param notCondition
 */
void ExtractGeneralASTVisitor::visitNotCondition(shared_ptr<NotCondition> notCondition) {
    // iterate into expressions
    notCondition->getConditionalExpression()->accept(shared_from_this());
}

/**
 * We visit an And condition and traverse both conditional expression to accept a visitor
 * @param andCondition
 */
void ExtractGeneralASTVisitor::visitAndCondition(shared_ptr<AndCondition> andCondition) {
    // iterate into expressions
    andCondition->getConditionalExpression1()->accept(shared_from_this());
    andCondition->getConditionalExpression2()->accept(shared_from_this());
}

/**
 * We visit an Or condition and traverse both conditional expression to accept a visitor
 * @param orCondition
 */
void ExtractGeneralASTVisitor::visitOrCondition(shared_ptr<OrCondition> orCondition) {
    // iterate into expressions
    orCondition->getConditionalExpression1()->accept(shared_from_this());
    orCondition->getConditionalExpression2()->accept(shared_from_this());
}



