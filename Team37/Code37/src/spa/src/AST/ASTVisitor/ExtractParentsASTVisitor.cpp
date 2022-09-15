#include "ExtractParentsASTVisitor.h"

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

ExtractParentsASTVisitor::ExtractParentsASTVisitor(shared_ptr<Storage> storage) { this->storage = storage; }

/**
 * We traverse all procedures in the source code to accept a visitor
 * @param sourceCode
 */
void ExtractParentsASTVisitor::visitSourceCode(shared_ptr<SourceCode> sourceCode) {
    vector<shared_ptr<Procedure>> procedures = sourceCode->getProcedures();

    for (auto procedure: procedures) {
        procedure->accept(shared_from_this());
    }
}

/**
 * We traverse all statements in the procedure to accept a visitor
 * @param procedure
 */
void ExtractParentsASTVisitor::visitProcedure(shared_ptr<Procedure> procedure) {
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
void ExtractParentsASTVisitor::visitReadStatement(shared_ptr<ReadStatement> readStmt) {
}

/**
 * We visit a print statement and store it as a NameExpression as it contains a variable
 * @param printStmt
 */
void ExtractParentsASTVisitor::visitPrintStatement(shared_ptr<PrintStatement> printStmt) {
}

/**
 *
 * @param callStmt
 */
void ExtractParentsASTVisitor::visitCallStatement(shared_ptr<CallStatement> callStmt) {
}

/**
 * We traverse all the statements in a while loop to accept a visitor
 * @param whileStmt
 */
void ExtractParentsASTVisitor::visitWhileStatement(shared_ptr<WhileStatement> whileStmt) {
    int parentLineNum = whileStmt->getLineNum();

    vector<shared_ptr<Statement>> statements = whileStmt->getStatements();

    for (auto statement: statements) {
        // store Parent relationship
        this->storage->storeRelation(parentLineNum, statement->getLineNum(), true, PARENT);

        // iterate children
        statement->accept(shared_from_this());
    }
}

/**
 * We traverse all the statements in if-else to accept a visitor
 * @param ifStmt
 */
void ExtractParentsASTVisitor::visitIfStatement(shared_ptr<IfStatement> ifStmt) {
    int parentLineNum = ifStmt->getLineNum();

    vector<shared_ptr<Statement>> thenStmts = ifStmt->getThenStatements();
    for (auto statement: thenStmts) {
        // store Parent relationship
        this->storage->storeRelation(parentLineNum, statement->getLineNum(), true, PARENT);

        // iterate children
        statement->accept(shared_from_this());
    }

    vector<shared_ptr<Statement>> elseStmts = ifStmt->getElseStatements();
    for (auto statement: elseStmts) {
        // store Parent relationship
        this->storage->storeRelation(parentLineNum, statement->getLineNum(), true, PARENT);

        // iterate children
        statement->accept(shared_from_this());
    }
}

/**
 * We visit an assign statement and store it as a NameExpression as it contains a variable
 * @param assignStmt
 */
void ExtractParentsASTVisitor::visitAssignStatement(shared_ptr<AssignStatement> assignStmt) {

}

// RelationalFactor

void ExtractParentsASTVisitor::visitNameExpression(shared_ptr<NameExpression> nameExpr) {

}

/**
 * We visit a constant expression and store it as a constant
 * @param constantExpr
 */
void ExtractParentsASTVisitor::visitConstantExpression(shared_ptr<ConstantExpression> constantExpr) {

}

/**
 * We visit an operated expression and traverse both the expression to accept a visitor
 * @param operatedExpr
 */
void ExtractParentsASTVisitor::visitOperatedExpression(shared_ptr<OperatedExpression> operatedExpr) {

}

// ConditionalExpression

/**
 * We visit a relational expression and traverse both relational factors to accept a visitor
 * @param relationalExpr
 */
void ExtractParentsASTVisitor::visitRelationalExpression(shared_ptr<RelationalExpression> relationalExpr) {

}

/**
 * We visit a not condition and traverse its conditional expression to accept a visitor
 * @param notCondition
 */
void ExtractParentsASTVisitor::visitNotCondition(shared_ptr<NotCondition> notCondition) {
}

/**
 * We visit an And condition and traverse both conditional expression to accept a visitor
 * @param andCondition
 */
void ExtractParentsASTVisitor::visitAndCondition(shared_ptr<AndCondition> andCondition) {

}

/**
 * We visit an Or condition and traverse both conditional expression to accept a visitor
 * @param orCondition
 */
void ExtractParentsASTVisitor::visitOrCondition(shared_ptr<OrCondition> orCondition) {

}