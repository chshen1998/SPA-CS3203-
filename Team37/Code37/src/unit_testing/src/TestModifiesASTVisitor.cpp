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


TEST_CASE("Modified assign statements") {
    shared_ptr<SourceCode> sc = make_shared<SourceCode>("Filename.txt");
    shared_ptr<Procedure> procedure = make_shared<Procedure>(sc, "Test Procedure");
    shared_ptr<RelationalFactor> rf = make_shared<ConstantExpression>(nullptr, 1);


    shared_ptr<AssignStatement> assignStmt = make_shared<AssignStatement>(procedure, "c", rf);

    shared_ptr<Storage> storage = make_shared<Storage>();

    procedure->addStatement(assignStmt);


    sc->addProcedure(procedure);

    // We start by traversing the AST
    storage->storeAST(sc);


    vector<string> modifiedVariables = storage->forwardRetrieveRelation(assignStmt->getLineNum(), MODIFIESSV);

    REQUIRE(modifiedVariables.size() == 1);
}

TEST_CASE("Modified read statements") {
    shared_ptr<SourceCode> sc = make_shared<SourceCode>("Filename.txt");
    shared_ptr<Procedure> procedure = make_shared<Procedure>(sc, "Test Procedure");


    shared_ptr<ReadStatement> readStmt = make_shared<ReadStatement>(procedure, "b");

    shared_ptr<Storage> storage = make_shared<Storage>();

    procedure->addStatement(readStmt);

    sc->addProcedure(procedure);

    // We start by traversing the AST
    storage->storeAST(sc);

    vector<string> modifiedVariables = storage->forwardRetrieveRelation(readStmt->getLineNum(), MODIFIESSV);

    REQUIRE(modifiedVariables.size() == 1);
}

TEST_CASE("Modified while statements") {
//    TODO: check with SP on NameExpression
    shared_ptr<SourceCode> sc = make_shared<SourceCode>("Filename.txt");
    shared_ptr<Procedure> procedure = make_shared<Procedure>(sc, "Test Procedure");
    shared_ptr<NameExpression> nameExpr = make_shared<NameExpression>(nullptr, "c");
    shared_ptr<ConstantExpression> constExpr = make_shared<ConstantExpression>(nullptr, 11);

    shared_ptr<ConditionalExpression> expr = make_shared<RelationalExpression>(
            nullptr,
            RelationalOperator::LESS_THAN,
            nameExpr,
            constExpr);

    shared_ptr<RelationalFactor> rf = make_shared<ConstantExpression>(nullptr, 1);
    shared_ptr<AssignStatement> assignStmt = make_shared<AssignStatement>(procedure, "c", rf);
    shared_ptr<WhileStatement> whileStmt = make_shared<WhileStatement>(procedure, expr);

    nameExpr->setParent(expr);
    constExpr->setParent(expr);
    expr->setParent(whileStmt);
    rf->setParent(assignStmt);


    shared_ptr<Storage> storage = make_shared<Storage>();

    procedure->addStatement(whileStmt);
    sc->addProcedure(procedure);

    // We start by traversing the AST
    storage->storeAST(sc);

    vector<string> modifiedVariables = storage->forwardRetrieveRelation(whileStmt->getLineNum(), MODIFIESSV);

    REQUIRE(modifiedVariables.size() == 1);
}

TEST_CASE("Modifies call statements") {
    /**
     * In this testcase, procedure 1 only contains a call stmt on procedure 2, we are testing if we are able to store
     * the variables from procedure 2 into Modifies(c,v) even though procedure 2 is traversed later on in the evaluation
     */
    shared_ptr<SourceCode> sc = make_shared<SourceCode>("Filename.txt");
    shared_ptr<Procedure> procedure1 = make_shared<Procedure>(sc, "first procedure");
    shared_ptr<Procedure> procedure2 = make_shared<Procedure>(sc, "second procedure");

    // first procedure calls on second procedure
    shared_ptr<CallStatement> callStmt = make_shared<CallStatement>(procedure1, "second procedure");
    shared_ptr<Storage> storage = make_shared<Storage>();

    shared_ptr<ReadStatement> readStmt1 = make_shared<ReadStatement>(procedure2, "a");
    shared_ptr<ReadStatement> readStmt2 = make_shared<ReadStatement>(procedure2, "b");
    shared_ptr<ReadStatement> readStmt3 = make_shared<ReadStatement>(procedure2, "c");

    procedure1->addStatement(callStmt);

    procedure2->addStatement(readStmt1);
    procedure2->addStatement(readStmt2);
    procedure2->addStatement(readStmt3);

    sc->addProcedure(procedure1);
    sc->addProcedure(procedure2);

    // We start by traversing the AST
    storage->storeAST(sc);

    vector<string> modifiedVariable = storage->forwardRetrieveRelation("first procedure", MODIFIESPV);

    REQUIRE(modifiedVariable.size() == 3);
}
