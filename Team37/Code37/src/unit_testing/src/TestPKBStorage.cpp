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

TEST_CASE("Storage - Stmt-Stmt Relation") {
    shared_ptr<Storage> store = make_shared<Storage>();

    // ASSUMES AST HAS 3 STMTS - CHANGE IF IMPLEMENTATION CHANGED
    shared_ptr<SourceCode> AST = make_shared<SourceCode>("../../../Test37/easy.txt");
    store->storeAST(AST);

    // Initally Empty
    REQUIRE(!store->retrieveRelation(1, 1, PARENT));
    REQUIRE(!store->retrieveRelation(1, 2, PARENT));
    REQUIRE(!store->retrieveRelation(1, 3, PARENT));
    REQUIRE(!store->retrieveRelation(1, 1, FOLLOWS));
    REQUIRE(!store->retrieveRelation(1, 2, FOLLOWS));
    REQUIRE(!store->retrieveRelation(1, 3, FOLLOWS));

    // Parent
    store->storeRelation(1, 2, true, PARENT);
    store->storeRelation(2, 3, true, PARENT);

    vector<int> cmp_1{ 2 };
    vector<int> cmp_2{ 3 };
    vector<int> cmp_3{};
    REQUIRE(store->forwardRetrieveRelation(1, PARENT) == cmp_1);
    REQUIRE(store->forwardRetrieveRelation(2, PARENT) == cmp_2);
    REQUIRE(store->forwardRetrieveRelation(3, PARENT) == cmp_3);

    cmp_1 = {};
    cmp_2 = { 1 };
    cmp_3 = { 2 };
    REQUIRE(store->reverseRetrieveRelation(1, PARENT) == cmp_1);
    REQUIRE(store->reverseRetrieveRelation(2, PARENT) == cmp_2);
    REQUIRE(store->reverseRetrieveRelation(3, PARENT) == cmp_3);

    store->buildStar(PARENT);

    vector<int> cmp_star_1{ 2, 3 };
    vector<int> cmp_star_2{ 3 };
    vector<int> cmp_star_3{};

    REQUIRE(store->forwardRetrieveRelation(1, PARENTS) == cmp_star_1);
    REQUIRE(store->forwardRetrieveRelation(2, PARENTS) == cmp_star_2);
    REQUIRE(store->forwardRetrieveRelation(3, PARENTS) == cmp_star_3);

    // Follows
    store->storeRelation(1, 2, true, FOLLOWS);
    store->storeRelation(2, 3, true, FOLLOWS);

    cmp_1 = { 2 };
    cmp_2 = { 3 };
    cmp_3 = {};
    REQUIRE(store->forwardRetrieveRelation(1, FOLLOWS) == cmp_1);
    REQUIRE(store->forwardRetrieveRelation(2, FOLLOWS) == cmp_2);
    REQUIRE(store->forwardRetrieveRelation(3, FOLLOWS) == cmp_3);

    cmp_1 = {};
    cmp_2 = { 1 };
    cmp_3 = { 2 };
    REQUIRE(store->reverseRetrieveRelation(1, FOLLOWS) == cmp_1);
    REQUIRE(store->reverseRetrieveRelation(2, FOLLOWS) == cmp_2);
    REQUIRE(store->reverseRetrieveRelation(3, FOLLOWS) == cmp_3);

    store->buildStar(FOLLOWS);


    REQUIRE(store->forwardRetrieveRelation(1, FOLLOWSS) == cmp_star_1);
    REQUIRE(store->forwardRetrieveRelation(2, FOLLOWSS) == cmp_star_2);
    REQUIRE(store->forwardRetrieveRelation(3, FOLLOWSS) == cmp_star_3);

}

TEST_CASE("Storage - Stmt-Var Relation") {
    shared_ptr<Storage> store = make_shared<Storage>();

    // Initally Empty
    REQUIRE(!store->retrieveRelation(1, "x", USESV));
    REQUIRE(store->forwardRetrieveRelation(1, USESV).size() == 0);
    REQUIRE(store->reverseRetrieveRelation("x", USESV).size() == 0);

    REQUIRE(!store->retrieveRelation(1, "x", MODIFIESV));
    REQUIRE(store->forwardRetrieveRelation(1, MODIFIESV).size() == 0);
    REQUIRE(store->reverseRetrieveRelation("x", MODIFIESV).size() == 0);

    
    vector<string> cmp_1 = { "x" };
    vector<string> cmp_2 = { "x", "y" };
    vector<string> cmp_3 = { "y", "z" };

    vector<int> cmp_x = { 1, 2 };
    vector<int> cmp_y = { 2, 3 };
    vector<int> cmp_z = { 3 };

    // UsesV
    store->storeRelation(1, "x", USESV);
    store->storeRelation(2, "x", USESV);
    store->storeRelation(2, "y", USESV);
    store->storeRelation(3, "y", USESV);
    store->storeRelation(3, "z", USESV);

    REQUIRE(store->retrieveRelation(1, "x", USESV));
    REQUIRE(store->retrieveRelation(2, "y", USESV));
    REQUIRE(!store->retrieveRelation(1, "z", USESV));

    REQUIRE(store->forwardRetrieveRelation(1, USESV) == cmp_1);
    REQUIRE(store->forwardRetrieveRelation(2, USESV) == cmp_2);
    REQUIRE(store->forwardRetrieveRelation(3, USESV) == cmp_3);

    REQUIRE(store->reverseRetrieveRelation("x", USESV) == cmp_x);
    REQUIRE(store->reverseRetrieveRelation("y", USESV) == cmp_y);
    REQUIRE(store->reverseRetrieveRelation("z", USESV) == cmp_z);

    // ModifiesV
    store->storeRelation(1, "x", MODIFIESV);
    store->storeRelation(2, "x", MODIFIESV);
    store->storeRelation(2, "y", MODIFIESV);
    store->storeRelation(3, "y", MODIFIESV);
    store->storeRelation(3, "z", MODIFIESV);

    REQUIRE(store->retrieveRelation(1, "x", MODIFIESV));
    REQUIRE(store->retrieveRelation(2, "y", MODIFIESV));
    REQUIRE(!store->retrieveRelation(1, "z", MODIFIESV));

    REQUIRE(store->forwardRetrieveRelation(1, MODIFIESV) == cmp_1);
    REQUIRE(store->forwardRetrieveRelation(2, MODIFIESV) == cmp_2);
    REQUIRE(store->forwardRetrieveRelation(3, MODIFIESV) == cmp_3);

    REQUIRE(store->reverseRetrieveRelation("x", MODIFIESV) == cmp_x);
    REQUIRE(store->reverseRetrieveRelation("y", MODIFIESV) == cmp_y);
    REQUIRE(store->reverseRetrieveRelation("z", MODIFIESV) == cmp_z);

}