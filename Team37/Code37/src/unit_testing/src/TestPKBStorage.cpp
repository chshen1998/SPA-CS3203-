#include "catch.hpp"

using namespace std;

#include "PKB/Storage.h"
#include "AST/SourceCode.h"
#include "AST/Expression/RelationalFactor/NameExpression.h"
#include "AST/Expression/RelationalFactor/ConstantExpression.h"


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

    shared_ptr<NameExpression> var_x = make_shared<NameExpression>(nullptr, "x");
    shared_ptr<NameExpression> var_y = make_shared<NameExpression>(nullptr, "y");

    store->storeVar(var_x);
    store->storeVar(var_y);

    REQUIRE(store->getAllVar().size() == 2);

    set<shared_ptr<TNode>> cmp_set;
    cmp_set.insert(var_x);
    cmp_set.insert(var_y);

    //Check for correct inner items
    REQUIRE(store->getAllVar() == cmp_set);

}

TEST_CASE("Storage - Constant") {
    shared_ptr<Storage> store = make_shared<Storage>();

    // Const set should be empty
    REQUIRE(store->getAllConst().empty());

    shared_ptr<ConstantExpression> const_0 = make_shared<ConstantExpression>(nullptr, 0);
    shared_ptr<ConstantExpression> const_1 = make_shared<ConstantExpression>(nullptr, -1);

    store->storeConst(const_0);
    store->storeConst(const_1);

    REQUIRE(store->getAllConst().size() == 2);

    set<shared_ptr<TNode>> cmp_set;
    cmp_set.insert(const_0);
    cmp_set.insert(const_1);

    //Check for correct inner items
    REQUIRE(store->getAllConst() == cmp_set);
}