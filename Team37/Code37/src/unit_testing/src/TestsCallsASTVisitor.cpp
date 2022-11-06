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

#include "PKB/Storage.h"

#include "SP/Tokenizer.h"

#include "catch.hpp"

using namespace std;

TEST_CASE("Calls procedures")
{
    // proc1 calls proc2 and proc3
    shared_ptr<SourceCode> sc = make_shared<SourceCode>("Filename.txt");
    shared_ptr<Procedure> procedure1 = make_shared<Procedure>(sc, "first procedure");
    shared_ptr<Procedure> procedure2 = make_shared<Procedure>(sc, "second procedure");
    shared_ptr<Procedure> procedure3 = make_shared<Procedure>(sc, "third procedure");

    shared_ptr<CallStatement> callStmt1 = make_shared<CallStatement>(procedure1, "second procedure");
    shared_ptr<CallStatement> callStmt2 = make_shared<CallStatement>(procedure1, "third procedure");

    shared_ptr<Storage> storage = make_shared<Storage>();

    procedure1->addStatement(callStmt1);
    procedure1->addStatement(callStmt2);

    sc->addProcedure(procedure1);
    sc->addProcedure(procedure2);

    // We start by traversing the AST
    storage->storeAST(sc);

    vector<string> callsRelationship = storage->forwardRetrieveRelation("first procedure", CALLS);

    REQUIRE(callsRelationship.size() == 2);
}

TEST_CASE("Calls star procedures")
{
    // proc1 calls proc2, proc2 calls on proc3
    shared_ptr<SourceCode> sc = make_shared<SourceCode>("Filename.txt");
    shared_ptr<Procedure> procedure1 = make_shared<Procedure>(sc, "first procedure");
    shared_ptr<Procedure> procedure2 = make_shared<Procedure>(sc, "second procedure");
    shared_ptr<Procedure> procedure3 = make_shared<Procedure>(sc, "third procedure");

    shared_ptr<CallStatement> callStmt1 = make_shared<CallStatement>(procedure1, "second procedure");
    shared_ptr<CallStatement> callStmt2 = make_shared<CallStatement>(procedure2, "third procedure");

    shared_ptr<Storage> storage = make_shared<Storage>();

    procedure1->addStatement(callStmt1);
    procedure1->addStatement(callStmt2);

    sc->addProcedure(procedure1);
    sc->addProcedure(procedure2);

    // We start by traversing the AST
    storage->storeAST(sc);

    vector<string> callsRelationship = storage->forwardRetrieveRelation("first procedure", CALLS);
    vector<string> callsStarRelationship = storage->forwardRetrieveRelation("first procedure", CALLSS);

    REQUIRE(callsRelationship.size() == 1);
    REQUIRE(callsStarRelationship.size() == 2);
}