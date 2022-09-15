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

#include "PKB/Storage.h"

#include "catch.hpp"

using namespace std;


TEST_CASE("Follow statements") {
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

    vector<int> follower1 = storage->forwardRetrieveRelation(1, FOLLOWS);
    vector<int> follower2 = storage->forwardRetrieveRelation(2, FOLLOWS);
    vector<int> follower3 = storage->forwardRetrieveRelation(3, FOLLOWS);

    REQUIRE(follower1[0] == 2);
    REQUIRE(follower2[0] == 3);
    REQUIRE(follower3[0] == 4);
}

TEST_CASE("Follow Star statements") {
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

    vector<int> followers1 = storage->forwardRetrieveRelation(1, FOLLOWSS);
    vector<int> followers2 = storage->forwardRetrieveRelation(2, FOLLOWSS);
    vector<int> followers3 = storage->forwardRetrieveRelation(3, FOLLOWSS);

    REQUIRE(followers1.size() == 3);
    REQUIRE(followers2.size() == 2);
    REQUIRE(followers3.size() == 1);
}