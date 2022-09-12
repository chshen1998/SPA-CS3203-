#include "catch.hpp"

using namespace std;

#include "PKB/Storage.h"
#include "AST/SourceCode.h"
#include "AST/Expression/RelationalFactor/NameExpression.h"
#include "AST/Expression/RelationalFactor/ConstantExpression.h"
#include "AST/Expression/RelationalFactor/ConstantExpression.h"
#include "AST/Expression/RelationalFactor/NameExpression.h"
#include "AST/Statement/Statement.h"
#include "AST/Statement/AssignStatement.h"
#include "AST/Statement/CallStatement.h"
#include "AST/Statement/IfStatement.h"
#include "AST/Statement/PrintStatement.h"
#include "AST/Statement/ReadStatement.h"
#include "AST/Statement/WhileStatement.h"


TEST_CASE("Storage - AST") {
    shared_ptr<Storage> store = make_shared<Storage>();

    // AST should be null if unassigned
    REQUIRE(store->retrieveAST() == nullptr);

    shared_ptr<SourceCode> AST = make_shared<SourceCode>("../../../Test37/easy.txt");
    store->storeAST(AST);

    // Check if AST correctly stored
    REQUIRE(AST == store->retrieveAST());
}

TEST_CASE("Storage - Variable") {
    shared_ptr<Storage> store = make_shared<Storage>();

    // Var set should be empty
    REQUIRE(store->getAllVar().empty());

    NameExpression var_x = NameExpression(nullptr, "x");
    NameExpression var_y = NameExpression(nullptr, "y");

    store->storeVar(var_x);
    store->storeVar(var_y);

    REQUIRE(store->getAllVar().size() == 2);

    set<NameExpression> cmp_set;
    cmp_set.insert(var_x);
    cmp_set.insert(var_y);

    //Check for correct inner items
    REQUIRE(store->getAllVar() == cmp_set);

    // Check for repeated insertion of same item
    store->storeVar(var_x);
    REQUIRE(store->getAllVar().size() == 2);

    // Check for repeated insertion of different item
    NameExpression other_var_x = NameExpression(nullptr, "x");
    cmp_set.insert(other_var_x);
    REQUIRE(store->getAllVar().size() == 2);
}

TEST_CASE("Storage - Constant") {
    shared_ptr<Storage> store = make_shared<Storage>();

    // Const set should be empty
    REQUIRE(store->getAllConst().empty());

    ConstantExpression const_0 = ConstantExpression(nullptr, 0);
    ConstantExpression const_1 = ConstantExpression(nullptr, -1);

    store->storeConst(const_0);
    store->storeConst(const_1);

    REQUIRE(store->getAllConst().size() == 2);

    set<ConstantExpression> cmp_set;
    cmp_set.insert(const_0);
    cmp_set.insert(const_1);

    //Check for correct inner items
    REQUIRE(store->getAllConst() == cmp_set);

    // Check for repeated insertion of same item
    store->storeConst(const_1);
    REQUIRE(store->getAllConst().size() == 2);

    // Check for repeated insertion of different item
    ConstantExpression other_const_0 = ConstantExpression(nullptr, 0);
    cmp_set.insert(other_const_0);
    REQUIRE(store->getAllConst().size() == 2);
}

TEST_CASE("Storage - Statement") {
    shared_ptr<Storage> store = make_shared<Storage>();

    shared_ptr<AssignStatement> assignStmt = make_shared<AssignStatement>(nullptr, 1, "x", nullptr);
    shared_ptr<CallStatement> callStmt = make_shared<CallStatement>(nullptr, 2, "procedure_x");
    shared_ptr<IfStatement> ifStmt = make_shared<IfStatement>(nullptr, 3, nullptr);
    shared_ptr<PrintStatement> printStmt = make_shared<PrintStatement>(nullptr, 4, "x");
    shared_ptr<ReadStatement> readStmt = make_shared<ReadStatement>(nullptr, 5, "x");
    shared_ptr<WhileStatement> whileStmt = make_shared<WhileStatement>(nullptr, 6, nullptr);

    store->storeStmt(assignStmt);
    store->storeStmt(callStmt);
    store->storeStmt(ifStmt);
    store->storeStmt(printStmt);
    store->storeStmt(readStmt);
    store->storeStmt(whileStmt);

    set<shared_ptr<Statement>> cmp_set;
    cmp_set.insert(assignStmt);
    cmp_set.insert(callStmt);
    cmp_set.insert(ifStmt);
    cmp_set.insert(printStmt);
    cmp_set.insert(readStmt);
    cmp_set.insert(whileStmt);

    REQUIRE(store->getAllStmt().size() == 6);
    REQUIRE(store->getAllStmt() == cmp_set);

    // Test repeated insertion
    store->storeStmt(assignStmt);
    REQUIRE(store->getAllStmt().size() == 6);
}