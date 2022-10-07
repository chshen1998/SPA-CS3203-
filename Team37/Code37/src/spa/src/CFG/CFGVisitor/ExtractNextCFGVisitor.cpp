#include "ExtractNextCFGVisitor.h"

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

ExtractNextCFGVisitor::ExtractNextCFGVisitor(shared_ptr<Storage> storage) { this->storage = storage; }


/**
 * We traverse all procedures in the source code to accept a visitor
 * @param sourceCode
 */
void ExtractNextCFGVisitor::visitSourceCode(shared_ptr<SourceCode> sourceCode) {
    vector<shared_ptr<Procedure>> procedures = sourceCode->getProcedures();

    for (auto procedure: procedures) {
        procedure->accept(shared_from_this());
    }
}

/**
 * We traverse all statements in the procedure to accept a visitor
 * @param procedure
 */
void ExtractNextCFGVisitor::visitProcedure(shared_ptr<Procedure> procedure) {
    procedure->buildCFG(procedure->getProcedureName());
    shared_ptr<CFG> cfg = procedure->getCFG();
    shared_ptr<CFGNode> startCFGNode = cfg->getStartNode();

}