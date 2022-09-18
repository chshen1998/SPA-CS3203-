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
    shared_ptr<RelationalFactor> rf = make_shared<NameExpression>(nullptr, "b");

    shared_ptr<AssignStatement> assignStmt = make_shared<AssignStatement>(procedure, 1, "a", rf);

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


    shared_ptr<PrintStatement> readStmt = make_shared<PrintStatement>(procedure, 1, "b");

    shared_ptr<Storage> storage = make_shared<Storage>();

    procedure->addStatement(readStmt);

    sc->addProcedure(procedure);

    // We start by traversing the AST
    storage->storeAST(sc);

    vector<string> usesVariables = storage->forwardRetrieveRelation(1, USESV);

    REQUIRE(usesVariables.size() == 1);
}

//TEST_CASE("Uses while statements") {
////    TODO: check with SP on NameExpression
//    shared_ptr<SourceCode> sc = make_shared<SourceCode>("Filename.txt");
//    shared_ptr<Procedure> procedure = make_shared<Procedure>(sc, "Test Procedure");
//    shared_ptr<ConditionalExpression> expr = make_shared<RelationalExpression>(
//            nullptr,
//            RelationalOperator::LESS_THAN,
//            make_shared<NameExpression>(nullptr, "c"),
//            make_shared<ConstantExpression>(nullptr, 11));
//
//    shared_ptr<RelationalFactor> rf = make_shared<ConstantExpression>(nullptr, 1);
//    shared_ptr<AssignStatement> assignStmt = make_shared<AssignStatement>(procedure, 1, "c", rf);
//
//    shared_ptr<WhileStatement> whileStmt = make_shared<WhileStatement>(procedure, 2, expr);
//
//    shared_ptr<Storage> storage = make_shared<Storage>();
//
//    procedure->addStatement(whileStmt);
//    sc->addProcedure(procedure);
//
//    // We start by traversing the AST
//    storage->storeAST(sc);
//
//    vector<string> modifiedVariables = storage->forwardRetrieveRelation(1, MODIFIESV);
//
//    REQUIRE(modifiedVariables.size() == 1);
//}

