#include "ExtractFollowsASTVisitor.h"

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

ExtractFollowsASTVisitor::ExtractFollowsASTVisitor(shared_ptr<Storage> storage) { this->storage = storage; }

/**
 * We traverse all procedures in the source code to accept a visitor
 * @param sourceCode
 */
void ExtractFollowsASTVisitor::visitSourceCode(shared_ptr<SourceCode> sourceCode) {
    vector<shared_ptr<Procedure>> procedures = sourceCode->getProcedures();

    for (auto procedure: procedures) {
        procedure->accept(shared_from_this());
    }
}

/**
 * We traverse all statements in the procedure to accept a visitor
 * @param procedure
 */
void ExtractFollowsASTVisitor::visitProcedure(shared_ptr<Procedure> procedure) {
    vector<shared_ptr<Statement>> statements = procedure->getStatements();

    //storing Follows relationship, we loop from first to second last element and store (index,index+1)
    for (int index = 0; index < statements.size() - 1; index++) {
        if (statements[index] == nullptr || statements[index + 1] == nullptr) {
            continue;
        }
        int followeeLineNum = statements[index]->getLineNum();
        int followerLineNum = statements[index + 1]->getLineNum();
        this->storage->storeRelation(followeeLineNum, followerLineNum, FOLLOWS);
    }

    // iterate statements
    for (auto statement: statements) {
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
void ExtractFollowsASTVisitor::visitReadStatement(shared_ptr<ReadStatement> readStmt) {

}

/**
 * We visit a print statement and store it as a NameExpression as it contains a variable
 * @param printStmt
 */
void ExtractFollowsASTVisitor::visitPrintStatement(shared_ptr<PrintStatement> printStmt) {

}

/**
 *
 * @param callStmt
 */
void ExtractFollowsASTVisitor::visitCallStatement(shared_ptr<CallStatement> callStmt) {

}

/**
 * We traverse all the statements in a while loop to accept a visitor
 * @param whileStmt
 */
void ExtractFollowsASTVisitor::visitWhileStatement(shared_ptr<WhileStatement> whileStmt) {
    vector<shared_ptr<Statement>> statements = whileStmt->getStatements();

    // loop through all nested statements
    // storing Follows relationship, we loop from first to second last element and store (index,index+1)
    for (int index = 0; index < (int) statements.size() - 1; index++) {
        int followeeLineNum = statements[index]->getLineNum();
        int followerLineNum = statements[index + 1]->getLineNum();
        this->storage->storeRelation(followeeLineNum, followerLineNum, FOLLOWS);
    }

    for (auto statement: statements) {
        // iterate children
        statement->accept(shared_from_this());
    }
}

/**
 * We traverse all the statements in if-else to accept a visitor
 * @param ifStmt
 */
void ExtractFollowsASTVisitor::visitIfStatement(shared_ptr<IfStatement> ifStmt) {
    vector<shared_ptr<Statement>> thenStmts = ifStmt->getThenStatements();

    // loop through all nested then statements
    // storing Follows relationship, we loop from first to second last element and store (index,index+1)
    for (int index = 0; index < (int) thenStmts.size() - 1; index++) {
        int followeeLineNum = thenStmts[index]->getLineNum();
        int followerLineNum = thenStmts[index + 1]->getLineNum();
        this->storage->storeRelation(followeeLineNum, followerLineNum, FOLLOWS);
    }

    for (auto statement: thenStmts) {
        // iterate children
        statement->accept(shared_from_this());
    }

    vector<shared_ptr<Statement>> elseStmts = ifStmt->getElseStatements();

    // loop through all nested then statements
    // storing Follows relationship, we loop from first to second last element and store (index,index+1)
    for (int index = 0; index < (int) elseStmts.size() - 1; index++) {
        int followeeLineNum = elseStmts[index]->getLineNum();
        int followerLineNum = elseStmts[index + 1]->getLineNum();
        this->storage->storeRelation(followeeLineNum, followerLineNum, FOLLOWS);
    }

    for (auto statement: elseStmts) {
        // iterate children
        statement->accept(shared_from_this());
    }
}

/**
 * We visit an assign statement and store it as a NameExpression as it contains a variable
 * @param assignStmt
 */
void ExtractFollowsASTVisitor::visitAssignStatement(shared_ptr<AssignStatement> assignStmt) {

}

// RelationalFactor

void ExtractFollowsASTVisitor::visitNameExpression(shared_ptr<NameExpression> nameExpr) {

}

/**
 * We visit a constant expression and store it as a constant
 * @param constantExpr
 */
void ExtractFollowsASTVisitor::visitConstantExpression(shared_ptr<ConstantExpression> constantExpr) {

}

/**
 * We visit an operated expression and traverse both the expression to accept a visitor
 * @param operatedExpr
 */
void ExtractFollowsASTVisitor::visitOperatedExpression(shared_ptr<OperatedExpression> operatedExpr) {

}

// ConditionalExpression

/**
 * We visit a relational expression and traverse both relational factors to accept a visitor
 * @param relationalExpr
 */
void ExtractFollowsASTVisitor::visitRelationalExpression(shared_ptr<RelationalExpression> relationalExpr) {

}

/**
 * We visit a not condition and traverse its conditional expression to accept a visitor
 * @param notCondition
 */
void ExtractFollowsASTVisitor::visitNotCondition(shared_ptr<NotCondition> notCondition) {
}

/**
 * We visit an And condition and traverse both conditional expression to accept a visitor
 * @param andCondition
 */
void ExtractFollowsASTVisitor::visitAndCondition(shared_ptr<AndCondition> andCondition) {

}

/**
 * We visit an Or condition and traverse both conditional expression to accept a visitor
 * @param orCondition
 */
void ExtractFollowsASTVisitor::visitOrCondition(shared_ptr<OrCondition> orCondition) {

}