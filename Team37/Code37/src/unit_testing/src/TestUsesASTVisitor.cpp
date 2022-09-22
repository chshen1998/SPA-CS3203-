#include "AST/SourceCode.h"

#include "AST/Statement/ReadStatement.h"
#include "AST/Statement/PrintStatement.h"
#include "AST/Statement/CallStatement.h"
#include "AST/Statement/WhileStatement.h"
#include "AST/Statement/IfStatement.h"
#include "AST/Statement/AssignStatement.h"

#include "AST/Expression/ConditionalExpression/RelationalExpression.h"
#include "AST/Expression/RelationalFactor/ConstantExpression.h"

#include "AST/Operators/RelationalOperator.h"

#include "PKB/Storage.h"

#include "catch.hpp"

using namespace std;


TEST_CASE("Uses assign statements") {
    shared_ptr<SourceCode> sc = make_shared<SourceCode>("Filename.txt");
    shared_ptr<Procedure> procedure = make_shared<Procedure>(sc, "Test Procedure");
    // TODO: change nullptr to assignStmt
    shared_ptr<RelationalFactor> rf = make_shared<NameExpression>(nullptr, "b");

    shared_ptr<AssignStatement> assignStmt = make_shared<AssignStatement>(procedure, "a", rf);

    shared_ptr<Storage> storage = make_shared<Storage>();

    procedure->addStatement(assignStmt);

    sc->addProcedure(procedure);

    // We start by traversing the AST
    storage->storeAST(sc);

    vector<string> usesVariables = storage->forwardRetrieveRelation(1, USESV);

    REQUIRE(usesVariables.size() == 0);
}

TEST_CASE("Uses print statements") {
    shared_ptr<SourceCode> sc = make_shared<SourceCode>("Filename.txt");
    shared_ptr<Procedure> procedure = make_shared<Procedure>(sc, "Test Procedure");

    shared_ptr<PrintStatement> printStmt = make_shared<PrintStatement>(procedure, "b");

    shared_ptr<Storage> storage = make_shared<Storage>();

    procedure->addStatement(printStmt);

    sc->addProcedure(procedure);

    // We start by traversing the AST
    storage->storeAST(sc);

    vector<string> usesVariables = storage->forwardRetrieveRelation(printStmt->getLineNum(), USESV);

    REQUIRE(usesVariables.size() == 1);
}

TEST_CASE("Uses while statements") {
    shared_ptr<SourceCode> sc = make_shared<SourceCode>("Filename.txt");
    shared_ptr<Procedure> procedure = make_shared<Procedure>(sc, "Test Procedure");

    shared_ptr<NameExpression> nameExpr = make_shared<NameExpression>(nullptr, "c");
    shared_ptr<ConstantExpression> constExpr = make_shared<ConstantExpression>(nullptr, 11);

    shared_ptr<ConditionalExpression> expr = make_shared<RelationalExpression>(
            nullptr,
            RelationalOperator::LESS_THAN,
            nameExpr,
            constExpr);

    nameExpr->setParent(expr);
    constExpr->setParent(expr);

    shared_ptr<RelationalFactor> rf = make_shared<ConstantExpression>(nullptr, 1);
    shared_ptr<AssignStatement> assignStmt = make_shared<AssignStatement>(procedure, "c", rf);
    shared_ptr<WhileStatement> whileStmt = make_shared<WhileStatement>(procedure, expr);
    shared_ptr<Storage> storage = make_shared<Storage>();

    expr->setParent(whileStmt);
    rf->setParent(assignStmt);

    procedure->addStatement(whileStmt);
    sc->addProcedure(procedure);

    // We start by traversing the AST
    storage->storeAST(sc);

    vector<string> usesVariables = storage->forwardRetrieveRelation(whileStmt->getLineNum(), USESV);

    REQUIRE(usesVariables.size() == 1);
}

