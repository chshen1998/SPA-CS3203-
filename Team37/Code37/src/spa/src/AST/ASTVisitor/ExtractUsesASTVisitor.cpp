#include "ExtractUsesASTVisitor.h"

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

ExtractUsesASTVisitor::ExtractUsesASTVisitor(shared_ptr<Storage> storage) { this->storage = storage; }

/**
 * We traverse all procedures in the source code to accept a visitor
 * @param sourceCode
 */
void ExtractUsesASTVisitor::visitSourceCode(shared_ptr<SourceCode> sourceCode) {
    vector<shared_ptr<Procedure>> procedures = sourceCode->getProcedures();

    for (auto procedure: procedures) {
        procedure->accept(shared_from_this());
    }
}

/**
 * We traverse all statements in the procedure to accept a visitor
 * @param procedure
 */
void ExtractUsesASTVisitor::visitProcedure(shared_ptr<Procedure> procedure) {
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
 * We visit a read statement and do nothing
 * @param readStmt
 */
void ExtractUsesASTVisitor::visitReadStatement(shared_ptr<ReadStatement> readStmt) {
    // we do not store for read stmts
}


/**
 * We visit a print statement and stores Uses(pn,v)
 * @param printStmt
 */
void ExtractUsesASTVisitor::visitPrintStatement(shared_ptr<PrintStatement> printStmt) {
    string variable = printStmt->getVariableName();
    this->storage->storeRelation(printStmt->getLineNum(), variable, USESSV);
    this->visitParentAndStore(printStmt->getParent(), variable);
}

/**
 * We visit call statement
 * @param callStmt
 */
void ExtractUsesASTVisitor::visitCallStatement(shared_ptr<CallStatement> callStmt) {
    string calledProcedureName = callStmt->getProcedureName();
    int lineNum = callStmt->getLineNum();
    vector<string> storedVariablesInProcedure = this->storage->forwardRetrieveRelation(calledProcedureName,
                                                                                       USESPV);

    if (!storedVariablesInProcedure.empty()) {
        for (const auto &variable: storedVariablesInProcedure) {
            // store Uses(c,v)
            this->storage->storeRelation(lineNum, variable, USESSV);
        }
    } else {
        pair<int, string> lineNumProcedurePair(lineNum, calledProcedureName);
        this->storage->callStmtProcedureQueue.push_back(lineNumProcedurePair);
    }
}

/**
 * We traverse all the statements in a while loop to accept a visitor
 * @param whileStmt
 */
void ExtractUsesASTVisitor::visitWhileStatement(shared_ptr<WhileStatement> whileStmt) {
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
void ExtractUsesASTVisitor::visitIfStatement(shared_ptr<IfStatement> ifStmt) {
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

    // iterate into conditional expressions
    ifStmt->getConditionalExpression()->accept(shared_from_this());
}

/**
 * We visit an assign statement and visit its relational factors
 * @param assignStmt
 */
void ExtractUsesASTVisitor::visitAssignStatement(shared_ptr<AssignStatement> assignStmt) {
    assignStmt->getRelFactor()->accept(shared_from_this());
}

// RelationalFactor

/**
 * We visit a name expression and store up along its Parent nodes
 * @param nameExpr
 */
void ExtractUsesASTVisitor::visitNameExpression(shared_ptr<NameExpression> nameExpr) {
    this->visitParentAndStore(nameExpr->getParent(), nameExpr->getVarName());
}

/**
 * We visit a constant expression and do nothing
 * @param constantExpr
 */
void ExtractUsesASTVisitor::visitConstantExpression(shared_ptr<ConstantExpression> constantExpr) {
}

/**
 * We visit an operated expression
 * @param operatedExpr
 */
void ExtractUsesASTVisitor::visitOperatedExpression(shared_ptr<OperatedExpression> operatedExpr) {
    operatedExpr->getExpression1()->accept(shared_from_this());
    operatedExpr->getExpression2()->accept(shared_from_this());

}

// ConditionalExpression

/**
 * We visit a relational expression
 * @param relationalExpr
 */
void ExtractUsesASTVisitor::visitRelationalExpression(shared_ptr<RelationalExpression> relationalExpr) {
    relationalExpr->getRelFactor1()->accept(shared_from_this());
    relationalExpr->getRelFactor2()->accept(shared_from_this());
}

/**
 * We visit a not condition
 * @param notCondition
 */
void ExtractUsesASTVisitor::visitNotCondition(shared_ptr<NotCondition> notCondition) {
    // iterate into conditional expressions
    notCondition->getConditionalExpression()->accept(shared_from_this());
}

/**
 * We visit an And condition
 * @param andCondition
 */
void ExtractUsesASTVisitor::visitAndCondition(shared_ptr<AndCondition> andCondition) {
    // iterate into conditional expressions
    andCondition->getConditionalExpression1()->accept(shared_from_this());
    andCondition->getConditionalExpression2()->accept(shared_from_this());

}

/**
 * We visit an Or condition
 * @param orCondition
 */
void ExtractUsesASTVisitor::visitOrCondition(shared_ptr<OrCondition> orCondition) {
    // iterate into conditional expressions
    orCondition->getConditionalExpression1()->accept(shared_from_this());
    orCondition->getConditionalExpression2()->accept(shared_from_this());
}

/**
 * helper function to visit all parent nodes and store Uses Relationship accordingly
 * for the respective variables and their parent containers
 * @param node
 */
void ExtractUsesASTVisitor::visitParentAndStore(shared_ptr<TNode> node, string variable) {
    while (node != nullptr) {
        // Assign Statement: Uses(a, v)
        if (dynamic_pointer_cast<AssignStatement>(node) != nullptr) {
            shared_ptr<AssignStatement> assignStmt = dynamic_pointer_cast<AssignStatement>(node);
            this->storage->storeRelation(assignStmt->getLineNum(), variable, USESSV);
        }

        // Read Statement: Uses(re, v)
        // JUST A SAFETY NET: shouldn't be called as Read Stmts have no children
        if (dynamic_pointer_cast<ReadStatement>(node) != nullptr) {
            shared_ptr<ReadStatement> readStmt = dynamic_pointer_cast<ReadStatement>(node);
            this->storage->storeRelation(readStmt->getLineNum(), variable, USESSV);
        }

        // If Statement: Uses(s, v)
        if (dynamic_pointer_cast<IfStatement>(node) != nullptr) {
            shared_ptr<IfStatement> ifStmt = dynamic_pointer_cast<IfStatement>(node);
            this->storage->storeRelation(ifStmt->getLineNum(), variable, USESSV);
        }

        // While Statement: Uses(s, v)
        if (dynamic_pointer_cast<WhileStatement>(node) != nullptr) {
            shared_ptr<WhileStatement> whileStmt = dynamic_pointer_cast<WhileStatement>(node);
            this->storage->storeRelation(whileStmt->getLineNum(), variable, USESSV);
        }

        // TODO: Procedure Statement: Uses(p, v)
        if (dynamic_pointer_cast<Procedure>(node) != nullptr) {
            shared_ptr<Procedure> procedure = dynamic_pointer_cast<Procedure>(node);
            this->storage->storeRelation(procedure->getProcedureName(), variable, USESPV);
        }

        // Call Statement: Uses(c, v)
        if (dynamic_pointer_cast<CallStatement>(node) != nullptr) {
        }

        // traverse upwards
        node = node->getParent();
    }

}
