#include "ExtractModifiesASTVisitor.h"


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

ExtractModifiesASTVisitor::ExtractModifiesASTVisitor(shared_ptr<Storage> storage) { this->storage = storage; }

/**
 * We traverse all procedures in the source code to accept a visitor
 * @param sourceCode
 */
void ExtractModifiesASTVisitor::visitSourceCode(shared_ptr<SourceCode> sourceCode) {
    vector<shared_ptr<Procedure>> procedures = sourceCode->getProcedures();

    for (auto procedure: procedures) {
        procedure->accept(shared_from_this());
    }
}

/**
 * We traverse all statements in the procedure to accept a visitor
 * @param procedure
 */
void ExtractModifiesASTVisitor::visitProcedure(shared_ptr<Procedure> procedure) {
    vector<shared_ptr<Statement>> statements = procedure->getStatements();
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
 * We visit a read statement and store Modifies(re, v)
 * @param readStmt
 */
void ExtractModifiesASTVisitor::visitReadStatement(shared_ptr<ReadStatement> readStmt) {
    string variable = readStmt->getVariableName();
    this->storage->storeRelation(readStmt->getLineNum(), variable, MODIFIESV);
    this->visitParentAndStore(readStmt, variable);
}


/**
 * We visit a print statement and do nothing
 * @param printStmt
 */
void ExtractModifiesASTVisitor::visitPrintStatement(shared_ptr<PrintStatement> printStmt) {
    // we do not store for print stmts
}

/**
 * We visit call statement
 * @param callStmt
 */
void ExtractModifiesASTVisitor::visitCallStatement(shared_ptr<CallStatement> callStmt) {
    //TODO
}

/**
 * We traverse all the statements in a while loop to accept a visitor
 * @param whileStmt
 */
void ExtractModifiesASTVisitor::visitWhileStatement(shared_ptr<WhileStatement> whileStmt) {
    // iterate into  statements
    vector<shared_ptr<Statement>> statements = whileStmt->getStatements();

    for (auto statement: statements) {
        statement->accept(shared_from_this());
    }

    // iterate into conditional expressions
    whileStmt->getConditionalExpression()->accept(shared_from_this());

}

/**
 * We traverse all the statements in if-else to accept a visitor
 * @param ifStmt
 */
void ExtractModifiesASTVisitor::visitIfStatement(shared_ptr<IfStatement> ifStmt) {
    vector<shared_ptr<Statement>> thenStmts = ifStmt->getThenStatements();
    for (auto statement: thenStmts) {
        // iterate children
        statement->accept(shared_from_this());
    }

    vector<shared_ptr<Statement>> elseStmts = ifStmt->getElseStatements();
    for (auto statement: elseStmts) {
        // iterate children
        statement->accept(shared_from_this());
    }
}

/**
 * We visit an assign statement and store it as a NameExpression as it contains a variable
 * @param assignStmt
 */
void ExtractModifiesASTVisitor::visitAssignStatement(shared_ptr<AssignStatement> assignStmt) {
    string variable = assignStmt->getVarName();

    this->storage->storeRelation(assignStmt->getLineNum(), variable, MODIFIESV);
    this->visitParentAndStore(assignStmt, variable);
}

// RelationalFactor

/**
 * We visit a name expression and do nothing
 * @param nameExpr
 */
void ExtractModifiesASTVisitor::visitNameExpression(shared_ptr<NameExpression> nameExpr) {
}

/**
 * We visit a constant expression and do nothing
 * @param constantExpr
 */
void ExtractModifiesASTVisitor::visitConstantExpression(shared_ptr<ConstantExpression> constantExpr) {
}

/**
 * We visit an operated expression and do nothing
 * @param operatedExpr
 */
void ExtractModifiesASTVisitor::visitOperatedExpression(shared_ptr<OperatedExpression> operatedExpr) {
}

// ConditionalExpression

/**
 * We visit a relational expression and do nothing
 * @param relationalExpr
 */
void ExtractModifiesASTVisitor::visitRelationalExpression(shared_ptr<RelationalExpression> relationalExpr) {
}

/**
 * We visit a not condition and do nothing
 * @param notCondition
 */
void ExtractModifiesASTVisitor::visitNotCondition(shared_ptr<NotCondition> notCondition) {
}

/**
 * We visit an And condition and do nothing
 * @param andCondition
 */
void ExtractModifiesASTVisitor::visitAndCondition(shared_ptr<AndCondition> andCondition) {

}

/**
 * We visit an Or condition and do nothing
 * @param orCondition
 */
void ExtractModifiesASTVisitor::visitOrCondition(shared_ptr<OrCondition> orCondition) {

}

/**
 * helper function to visit all parent nodes and store Modifies Relationship accordingly
 * for the respective variables and their parent containers
 * @param node
 */
void ExtractModifiesASTVisitor::visitParentAndStore(shared_ptr<TNode> node, string variable) {
    while (node != nullptr) {
        // Assign Statement: Modifies(a, v)
        if (dynamic_pointer_cast<AssignStatement>(node) != nullptr) {
            shared_ptr<AssignStatement> assignStmt = dynamic_pointer_cast<AssignStatement>(node);
            this->storage->storeRelation(assignStmt->getLineNum(), variable, MODIFIESV);
        }

        // Read Statement: Modifies(re, v)
        // JUST A SAFETY NET: shouldn't be called as Read Stmts have no children
        if (dynamic_pointer_cast<ReadStatement>(node) != nullptr) {
            shared_ptr<ReadStatement> readStmt = dynamic_pointer_cast<ReadStatement>(node);
            this->storage->storeRelation(readStmt->getLineNum(), variable, MODIFIESV);
        }

        // If Statement: Modifies(s, v)
        if (dynamic_pointer_cast<IfStatement>(node) != nullptr) {
            shared_ptr<IfStatement> ifStmt = dynamic_pointer_cast<IfStatement>(node);
            this->storage->storeRelation(ifStmt->getLineNum(), variable, MODIFIESV);
        }

        // While Statement: Modifies(s, v)
        if (dynamic_pointer_cast<WhileStatement>(node) != nullptr) {
            shared_ptr<WhileStatement> whileStmt = dynamic_pointer_cast<WhileStatement>(node);
            this->storage->storeRelation(whileStmt->getLineNum(), variable, MODIFIESV);
        }

        // TODO: Procedure Statement: Modifies(p, v)
        if (dynamic_pointer_cast<Procedure>(node) != nullptr) {
        }

        // Call Statement: Modifies(c, v)
        if (dynamic_pointer_cast<CallStatement>(node) != nullptr) {
        }

        // traverse upwards
        node = node->getParent();
    }

}
