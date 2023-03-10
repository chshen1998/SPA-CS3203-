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

#include "AST/ASTVisitor/ExtractGeneralASTVisitor.h"

#include "PKB/Storage.h"

#include "catch.hpp"

using namespace std;

TEST_CASE("Follow statements")
{
    shared_ptr<SourceCode> sc = make_shared<SourceCode>("Filename.txt");
    shared_ptr<Procedure> procedure = make_shared<Procedure>(sc, "Test Procedure");
    shared_ptr<ReadStatement> readStmt1 = make_shared<ReadStatement>(procedure, "a");
    shared_ptr<ReadStatement> readStmt2 = make_shared<ReadStatement>(procedure, "b");
    shared_ptr<ReadStatement> readStmt3 = make_shared<ReadStatement>(procedure, "c");
    shared_ptr<ReadStatement> readStmt4 = make_shared<ReadStatement>(procedure, "d");
    readStmt1->setParent(procedure);
    readStmt2->setParent(procedure);
    readStmt3->setParent(procedure);
    readStmt4->setParent(procedure);

    shared_ptr<Storage> storage = make_shared<Storage>();

    procedure->addStatement(readStmt1);
    procedure->addStatement(readStmt2);
    procedure->addStatement(readStmt3);
    procedure->addStatement(readStmt4);

    sc->addProcedure(procedure);

    // We start by traversing the AST
    storage->storeAST(sc);

    vector<int> follower1 = storage->forwardRetrieveRelation(readStmt1->getLineNum(), FOLLOWS);
    vector<int> follower2 = storage->forwardRetrieveRelation(readStmt2->getLineNum(), FOLLOWS);
    vector<int> follower3 = storage->forwardRetrieveRelation(readStmt3->getLineNum(), FOLLOWS);

    REQUIRE(follower1[0] == readStmt2->getLineNum());
    REQUIRE(follower2[0] == readStmt3->getLineNum());
    REQUIRE(follower3[0] == readStmt4->getLineNum());
}

TEST_CASE("Follow Star statements")
{
    shared_ptr<SourceCode> sc = make_shared<SourceCode>("Filename.txt");
    shared_ptr<Procedure> procedure = make_shared<Procedure>(sc, "Test Procedure");
    shared_ptr<ReadStatement> readStmt1 = make_shared<ReadStatement>(procedure, "a");
    shared_ptr<ReadStatement> readStmt2 = make_shared<ReadStatement>(procedure, "b");
    shared_ptr<ReadStatement> readStmt3 = make_shared<ReadStatement>(procedure, "c");
    shared_ptr<ReadStatement> readStmt4 = make_shared<ReadStatement>(procedure, "d");
    shared_ptr<Storage> storage = make_shared<Storage>();

    readStmt1->setParent(procedure);
    readStmt2->setParent(procedure);
    readStmt3->setParent(procedure);
    readStmt4->setParent(procedure);

    procedure->addStatement(readStmt1);
    procedure->addStatement(readStmt2);
    procedure->addStatement(readStmt3);
    procedure->addStatement(readStmt4);

    sc->addProcedure(procedure);

    // We start by traversing the AST
    storage->storeAST(sc);

    vector<int> followers1 = storage->forwardRetrieveRelation(readStmt1->getLineNum(), FOLLOWSS);
    vector<int> followers2 = storage->forwardRetrieveRelation(readStmt2->getLineNum(), FOLLOWSS);
    vector<int> followers3 = storage->forwardRetrieveRelation(readStmt3->getLineNum(), FOLLOWSS);

    REQUIRE(followers1.size() == 3);
    REQUIRE(followers2.size() == 2);
    REQUIRE(followers3.size() == 1);
}