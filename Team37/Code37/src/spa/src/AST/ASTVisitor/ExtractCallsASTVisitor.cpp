#include "ExtractCallsASTVisitor.h"

#include "AST/SourceCode.h"
#include "AST/Statement/AssignStatement.h"
#include "AST/Statement/CallStatement.h"
#include "AST/Statement/IfStatement.h"
#include "AST/Statement/PrintStatement.h"
#include "AST/Statement/ReadStatement.h"
#include "AST/Statement/WhileStatement.h"

#include "AST/Expression/RelationalFactor/ConstantExpression.h"
#include "AST/Expression/RelationalFactor/NameExpression.h"
#include "AST/Expression/RelationalFactor/OperatedExpression.h"

#include "AST/Expression/ConditionalExpression/AndCondition.h"
#include "AST/Expression/ConditionalExpression/NotCondition.h"
#include "AST/Expression/ConditionalExpression/OrCondition.h"
#include "AST/Expression/ConditionalExpression/RelationalExpression.h"

#include "PKB/Storage.h"

ExtractCallsASTVisitor::ExtractCallsASTVisitor(shared_ptr<Storage> storage) { this->storage = storage; }

/**
 * We traverse all procedures in the source code to accept a visitor
 * @param sourceCode
 */
void ExtractCallsASTVisitor::visitSourceCode(shared_ptr<SourceCode> sourceCode)
{
    vector<shared_ptr<Procedure>> procedures = sourceCode->getProcedures();

    for (auto procedure : procedures) {
        procedure->accept(shared_from_this());
    }
}

/**
 * We traverse all statements in the procedure to accept a visitor
 * @param procedure
 */
void ExtractCallsASTVisitor::visitProcedure(shared_ptr<Procedure> procedure)
{
    vector<shared_ptr<Statement>> statements = procedure->getStatements();

    // iterate statements
    for (auto statement : statements) {
        if (statement == nullptr) {
            continue;
        } else {
            statement->accept(shared_from_this());
        }
    }
}

// Statements

/**
 * We visit a read statement and store it as a NameExpression as it contains a variable
 * @param readStmt
 */
void ExtractCallsASTVisitor::visitReadStatement(shared_ptr<ReadStatement> readStmt)
{
}

/**
 * We visit a print statement and store it as a NameExpression as it contains a variable
 * @param printStmt
 */
void ExtractCallsASTVisitor::visitPrintStatement(shared_ptr<PrintStatement> printStmt)
{
}

/**
 *
 * @param callStmt
 */
void ExtractCallsASTVisitor::visitCallStatement(shared_ptr<CallStatement> callStmt)
{
    this->visitParentAndStore(callStmt, callStmt->getProcedureName());
}

/**
 * We traverse all the statements in a while loop to accept a visitor
 * @param whileStmt
 */
void ExtractCallsASTVisitor::visitWhileStatement(shared_ptr<WhileStatement> whileStmt)
{
    vector<shared_ptr<Statement>> statements = whileStmt->getStatements();

    for (auto statement : statements) {
        // iterate children
        statement->accept(shared_from_this());
    }
}

/**
 * We traverse all the statements in if-else to accept a visitor
 * @param ifStmt
 */
void ExtractCallsASTVisitor::visitIfStatement(shared_ptr<IfStatement> ifStmt)
{
    vector<shared_ptr<Statement>> thenStmts = ifStmt->getThenStatements();

    for (auto statement : thenStmts) {
        // iterate children
        statement->accept(shared_from_this());
    }

    vector<shared_ptr<Statement>> elseStmts = ifStmt->getElseStatements();

    for (auto statement : elseStmts) {
        // iterate children
        statement->accept(shared_from_this());
    }
}

/**
 * We visit an assign statement and store it as a NameExpression as it contains a variable
 * @param assignStmt
 */
void ExtractCallsASTVisitor::visitAssignStatement(shared_ptr<AssignStatement> assignStmt)
{
}

// RelationalFactor

void ExtractCallsASTVisitor::visitNameExpression(shared_ptr<NameExpression> nameExpr)
{
}

/**
 * We visit a constant expression and store it as a constant
 * @param constantExpr
 */
void ExtractCallsASTVisitor::visitConstantExpression(shared_ptr<ConstantExpression> constantExpr)
{
}

/**
 * We visit an operated expression and traverse both the expression to accept a visitor
 * @param operatedExpr
 */
void ExtractCallsASTVisitor::visitOperatedExpression(shared_ptr<OperatedExpression> operatedExpr)
{
}

// ConditionalExpression

/**
 * We visit a relational expression and traverse both relational factors to accept a visitor
 * @param relationalExpr
 */
void ExtractCallsASTVisitor::visitRelationalExpression(shared_ptr<RelationalExpression> relationalExpr)
{
}

/**
 * We visit a not condition and traverse its conditional expression to accept a visitor
 * @param notCondition
 */
void ExtractCallsASTVisitor::visitNotCondition(shared_ptr<NotCondition> notCondition)
{
}

/**
 * We visit an And condition and traverse both conditional expression to accept a visitor
 * @param andCondition
 */
void ExtractCallsASTVisitor::visitAndCondition(shared_ptr<AndCondition> andCondition)
{
}

/**
 * We visit an Or condition and traverse both conditional expression to accept a visitor
 * @param orCondition
 */
void ExtractCallsASTVisitor::visitOrCondition(shared_ptr<OrCondition> orCondition)
{
}

/**
 * helper function to visit all parent nodes and store Modifies Relationship accordingly
 * for the respective variables and their parent containers
 * @param node
 */
void ExtractCallsASTVisitor::visitParentAndStore(shared_ptr<TNode> node, string procName)
{
    while (node != nullptr) {
        // Procedure for Calls(p, q)
        if (dynamic_pointer_cast<Procedure>(node) != nullptr) {
            shared_ptr<Procedure> procedure = dynamic_pointer_cast<Procedure>(node);
            this->storage->storeRelation(procedure->getProcedureName(), procName, CALLS);
        }

        // traverse upwards
        node = node->getParent();
    }
}