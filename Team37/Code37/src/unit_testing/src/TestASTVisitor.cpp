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

#include "AST/ASTVisitor/ExtractGeneralASTVisitor.h"

#include "PKB/PKB.h"
#include "PKB/Storage.h"

#include "catch.hpp"

using namespace std;


TEST_CASE("Read Statements") {
    shared_ptr<SourceCode> sc = make_shared<SourceCode>("Filename.txt");
    shared_ptr<Procedure> procedure = make_shared<Procedure>(sc, "Test Procedure");
    shared_ptr<ReadStatement> readStmt1 = make_shared<ReadStatement>(procedure, 1, "a");
    shared_ptr<ReadStatement> readStmt2 = make_shared<ReadStatement>(procedure, 2, "b");
    shared_ptr<ReadStatement> readStmt3 = make_shared<ReadStatement>(procedure, 3, "c");
    shared_ptr<ReadStatement> readStmt4 = make_shared<ReadStatement>(procedure, 4, "d");
    shared_ptr<Storage> storage = make_shared<Storage>();

    procedure->addStatement(readStmt1);
    procedure->addStatement(readStmt2);
    procedure->addStatement(readStmt3);
    procedure->addStatement(readStmt4);

    sc->addProcedure(procedure);

    // We start by traversing the AST
    storage->storeAST(sc);

    REQUIRE(storage->getAllVar().size() == 4);
}

TEST_CASE("Print Statements") {
    shared_ptr<SourceCode> sc = make_shared<SourceCode>("Filename.txt");
    shared_ptr<Procedure> procedure = make_shared<Procedure>(sc, "Test Procedure");
    shared_ptr<PrintStatement> printStmt1 = make_shared<PrintStatement>(procedure, 1, "a");
    shared_ptr<PrintStatement> printStmt2 = make_shared<PrintStatement>(procedure, 2, "b");
    shared_ptr<PrintStatement> printStmt3 = make_shared<PrintStatement>(procedure, 3, "c");
    shared_ptr<PrintStatement> printStmt4 = make_shared<PrintStatement>(procedure, 4, "d");
    shared_ptr<Storage> storage = make_shared<Storage>();

    procedure->addStatement(printStmt1);
    procedure->addStatement(printStmt2);
    procedure->addStatement(printStmt3);
    procedure->addStatement(printStmt4);

    sc->addProcedure(procedure);

    // We start by traversing the AST
    storage->storeAST(sc);

    REQUIRE(storage->getAllVar().size() == 4);
}

TEST_CASE("Constant Expression") {
    shared_ptr<SourceCode> sc = make_shared<SourceCode>("Filename.txt");
    shared_ptr<Procedure> procedure = make_shared<Procedure>(sc, "Test Procedure");
    shared_ptr<Storage> storage = make_shared<Storage>();
    shared_ptr<ConditionalExpression> expr1 = make_shared<RelationalExpression>(
            nullptr,
            RelationalOperator::EQUALS,
            make_shared<ConstantExpression>(nullptr, 10),
            make_shared<ConstantExpression>(nullptr, 11));
    shared_ptr<Statement> whileStmt =
            make_shared<WhileStatement>(procedure, 1, expr1);

    procedure->addStatement(whileStmt);


    sc->addProcedure(procedure);

    // We start by traversing the AST
    storage->storeAST(sc);

    REQUIRE(storage->getAllConst().size() == 2);
}