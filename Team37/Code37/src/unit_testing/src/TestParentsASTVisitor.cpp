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


TEST_CASE("Parent statements") {
    shared_ptr<SourceCode> sc = make_shared<SourceCode>("Filename.txt");
    shared_ptr<Procedure> procedure = make_shared<Procedure>(sc, "Test Procedure");
    shared_ptr<Storage> storage = make_shared<Storage>();
    shared_ptr<ConditionalExpression> expr1 = make_shared<RelationalExpression>(
            nullptr,
            RelationalOperator::EQUALS,
            make_shared<ConstantExpression>(nullptr, 10),
            make_shared<ConstantExpression>(nullptr, 11));
    shared_ptr<WhileStatement> whileStmt =
            make_shared<WhileStatement>(procedure, 1, expr1);
    shared_ptr<PrintStatement> printStmt1 = make_shared<PrintStatement>(procedure, 2, "a");
    shared_ptr<PrintStatement> printStmt2 = make_shared<PrintStatement>(procedure, 3, "b");
    shared_ptr<PrintStatement> printStmt3 = make_shared<PrintStatement>(procedure, 4, "c");
    shared_ptr<PrintStatement> printStmt4 = make_shared<PrintStatement>(procedure, 5, "d");

    whileStmt->addStatement(printStmt1);
    whileStmt->addStatement(printStmt2);
    whileStmt->addStatement(printStmt3);
    whileStmt->addStatement(printStmt4);

    procedure->addStatement(whileStmt);
    sc->addProcedure(procedure);


    // We start by traversing the AST
    storage->storeAST(sc);

    vector<int> numChild = storage->forwardRetrieveRelation(1, PARENT);


    REQUIRE(numChild.size() == 4);
}

/**
 * For this testcase we do not test double-nested loops, only single-nested (similar to the previous testcase)
 */
TEST_CASE("Parent Star statements") {
    shared_ptr<SourceCode> sc = make_shared<SourceCode>("Filename.txt");
    shared_ptr<Procedure> procedure = make_shared<Procedure>(sc, "Test Procedure");
    shared_ptr<Storage> storage = make_shared<Storage>();
    shared_ptr<ConditionalExpression> expr1 = make_shared<RelationalExpression>(
            nullptr,
            RelationalOperator::EQUALS,
            make_shared<ConstantExpression>(nullptr, 10),
            make_shared<ConstantExpression>(nullptr, 11));
    shared_ptr<WhileStatement> whileStmt =
            make_shared<WhileStatement>(procedure, 1, expr1);
    shared_ptr<PrintStatement> printStmt1 = make_shared<PrintStatement>(procedure, 2, "a");
    shared_ptr<PrintStatement> printStmt2 = make_shared<PrintStatement>(procedure, 3, "b");
    shared_ptr<PrintStatement> printStmt3 = make_shared<PrintStatement>(procedure, 4, "c");
    shared_ptr<PrintStatement> printStmt4 = make_shared<PrintStatement>(procedure, 5, "d");

    whileStmt->addStatement(printStmt1);
    whileStmt->addStatement(printStmt2);
    whileStmt->addStatement(printStmt3);
    whileStmt->addStatement(printStmt4);

    procedure->addStatement(whileStmt);
    sc->addProcedure(procedure);


    // We start by traversing the AST
    storage->storeAST(sc);

    vector<int> numChild = storage->forwardRetrieveRelation(1, PARENTS);


    REQUIRE(numChild.size() == 4);
}