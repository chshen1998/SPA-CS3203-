#include "catch.hpp"

using namespace std;

#include "PKB/Storage.h"
#include "AST/SourceCode.h"
#include "AST/Expression/RelationalFactor/NameExpression.h"
#include "AST/Expression/RelationalFactor/ConstantExpression.h"


void require(bool b);


TEST_CASE("Storage - AST") {
    shared_ptr<Storage> store = make_shared<Storage>();

    // AST should be null if unassigned
    require(store->retrieveAST() == nullptr);

    shared_ptr<SourceCode> AST = make_shared<SourceCode>("../../../Test37/easy.txt");
    store->storeAST(AST);

    // Check if AST correctly stored
    require(AST == store->retrieveAST());
}

TEST_CASE("Storage - Variable") {
    shared_ptr<Storage> store = make_shared<Storage>();

    // Var set should be empty
    require(store->getAllVar().empty());

    NameExpression var_x = NameExpression(nullptr, "x");
    NameExpression var_y = NameExpression(nullptr, "y");

    store->storeVar(var_x);
    store->storeVar(var_y);

    require(store->getAllVar().size() == 2);

    set<NameExpression> cmp_set;
    cmp_set.insert(var_x);
    cmp_set.insert(var_y);

    //Check for correct inner items
    require(store->getAllVar() == cmp_set);

    // Check for repeated insertion of same item
    store->storeVar(var_x);
    require(store->getAllVar().size() == 2);

    // Check for repeated insertion of different item
    NameExpression other_var_x = NameExpression(nullptr, "x");
    cmp_set.insert(other_var_x);
    require(store->getAllVar().size() == 2);
}

TEST_CASE("Storage - Constant") {
    shared_ptr<Storage> store = make_shared<Storage>();

    // Const set should be empty
    require(store->getAllConst().empty());

    ConstantExpression const_0 = ConstantExpression(nullptr, 0);
    ConstantExpression const_1 = ConstantExpression(nullptr, -1);

    store->storeConst(const_0);
    store->storeConst(const_1);

    require(store->getAllConst().size() == 2);

    set<ConstantExpression> cmp_set;
    cmp_set.insert(const_0);
    cmp_set.insert(const_1);

    //Check for correct inner items
    require(store->getAllConst() == cmp_set);

    // Check for repeated insertion of same item
    store->storeConst(const_1);
    require(store->getAllConst().size() == 2);

    // Check for repeated insertion of different item
    ConstantExpression other_const_0 = ConstantExpression(nullptr, 0);
    cmp_set.insert(other_const_0);
    require(store->getAllConst().size() == 2);
}