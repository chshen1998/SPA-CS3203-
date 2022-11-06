#include "AST/SourceCode.h"

#include "AST/Statement/AssignStatement.h"
#include "AST/Statement/CallStatement.h"
#include "AST/Statement/IfStatement.h"
#include "AST/Statement/PrintStatement.h"
#include "AST/Statement/ReadStatement.h"
#include "AST/Statement/WhileStatement.h"

#include "AST/Expression/ConditionalExpression/RelationalExpression.h"
#include "AST/Expression/RelationalFactor/ConstantExpression.h"

#include "AST/Operators/RelationalOperator.h"

#include "catch.hpp"
using namespace std;

TEST_CASE("1st Test")
{
    shared_ptr<SourceCode> sc = make_shared<SourceCode>("");
    shared_ptr<Procedure> procedure1 = make_shared<Procedure>(sc, "TestProcedure 1");
    shared_ptr<Procedure> procedure2 = make_shared<Procedure>(sc, "TestProcedure 2");

    sc->addProcedure(procedure1);
    sc->addProcedure(procedure2);

    REQUIRE(sc->getProcedures()[0] == procedure1);
    REQUIRE(sc->getProcedures()[1] == procedure2);
    // Negative case for sanity
    REQUIRE(sc->getProcedures()[1] != procedure1);

    REQUIRE(procedure1->getParent() == sc);
    REQUIRE(procedure2->getParent() == sc);
    REQUIRE(sc->getParent() == nullptr);

    shared_ptr<SourceCode> sc2 = make_shared<SourceCode>("Filename.txt");
    procedure1->setParent(sc2);
    REQUIRE(procedure1->getParent() == sc2);

    REQUIRE(sc->getFileName() == "");
    REQUIRE(sc2->getFileName() == "Filename.txt");
    REQUIRE(procedure1->getProcedureName() == "TestProcedure 1");
    REQUIRE(procedure2->getProcedureName() == "TestProcedure 2");
}

TEST_CASE("Simple Specific Statements")
{
    shared_ptr<SourceCode> sc = make_shared<SourceCode>("");
    shared_ptr<Procedure> procedure1 = make_shared<Procedure>(sc, "TestProcedure 1");

    sc->addProcedure(procedure1);

    shared_ptr<ConditionalExpression> expr1 = make_shared<RelationalExpression>(
        nullptr,
        RelationalOperator::EQUALS,
        make_shared<ConstantExpression>(nullptr, 10),
        make_shared<ConstantExpression>(nullptr, 11));

    shared_ptr<ConditionalExpression> expr2 = make_shared<RelationalExpression>(
        nullptr,
        RelationalOperator::GREATER_THAN_OR_EQUALS,
        make_shared<ConstantExpression>(nullptr, 12),
        make_shared<ConstantExpression>(nullptr, 12));

    shared_ptr<RelationalFactor> expr3 = make_shared<ConstantExpression>(nullptr, 1);

    // Initialize all types of statements
    shared_ptr<Statement> readStmt = make_shared<ReadStatement>(procedure1, "x");
    shared_ptr<Statement> printStmt = make_shared<PrintStatement>(procedure1, "y");
    shared_ptr<Statement> callStmt = make_shared<CallStatement>(procedure1, "z");
    shared_ptr<Statement> whileStmt = make_shared<WhileStatement>(procedure1, expr1);
    shared_ptr<Statement> ifStmt = make_shared<IfStatement>(procedure1, expr2);
    shared_ptr<Statement> assignStmt = make_shared<AssignStatement>(procedure1, "c", expr3);

    procedure1->addStatement(readStmt);
    procedure1->addStatement(printStmt);
    procedure1->addStatement(callStmt);
    procedure1->addStatement(whileStmt);
    procedure1->addStatement(ifStmt);
    procedure1->addStatement(assignStmt);

    vector<shared_ptr<Statement>> statements = procedure1->getStatements();
    REQUIRE(statements[0] == readStmt);
    REQUIRE(statements[1] == printStmt);
    REQUIRE(statements[2] == callStmt);
    REQUIRE(statements[3] == whileStmt);
    REQUIRE(statements[4] == ifStmt);
    REQUIRE(statements[5] == assignStmt);
}
