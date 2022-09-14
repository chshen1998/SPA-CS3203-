#include "catch.hpp"

using namespace std;

#include "PKB/Storage.h"
#include "PKB/QueryServicer.h"
#include "AST/SourceCode.h"
#include "AST/Expression/RelationalFactor/NameExpression.h"
#include "AST/Expression/RelationalFactor/ConstantExpression.h"



TEST_CASE("QueryServicer - Variable") {
    // Storage Stub
    shared_ptr<Storage> store = make_shared<Storage>();
    NameExpression var_x = NameExpression(nullptr, "x");
    NameExpression var_y = NameExpression(nullptr, "y");
    store->storeVar(var_x);
    store->storeVar(var_y);

    // Comparison Set
    set<NameExpression> cmp_set;
    cmp_set.insert(var_x);
    cmp_set.insert(var_y);

    shared_ptr<QueryServicer> queryService = make_shared<QueryServicer>(store);
    REQUIRE(queryService->getAllVar() == cmp_set);
}

TEST_CASE("QueryServicer - Constant") {
    // Storage Stub
    shared_ptr<Storage> store = make_shared<Storage>();
    ConstantExpression const_0 = ConstantExpression(nullptr, 0);
    ConstantExpression const_1 = ConstantExpression(nullptr, -1);
    store->storeConst(const_0);
    store->storeConst(const_1);
    
    // Comparison Set
    set<ConstantExpression> cmp_set;
    cmp_set.insert(const_0);
    cmp_set.insert(const_1);

    shared_ptr<QueryServicer> queryService = make_shared<QueryServicer>(store);
    REQUIRE(queryService->getAllConst() == cmp_set);
}

TEST_CASE("QueryServicer - Statements") {
    // Storage Stub
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

    shared_ptr<QueryServicer> queryService = make_shared<QueryServicer>(store);

    // All Statement
    REQUIRE(queryService->getAllStmt(StatementType::STATEMENT) == cmp_set);

    // Assign Statement
   set<shared_ptr<Statement>> retrievedAssign = queryService->getAllStmt(StatementType::ASSIGN);
   REQUIRE(retrievedAssign.size() == 1);
   REQUIRE(*(retrievedAssign.begin()) == assignStmt);

   // Call Statement
   set<shared_ptr<Statement>> retrievedCall = queryService->getAllStmt(StatementType::CALL);
   /*REQUIRE(retrievedCall.size() == 1);*/
   //REQUIRE(*(retrievedCall.begin()) == callStmt);

   // If Statement
   set<shared_ptr<Statement>> retrievedIf = queryService->getAllStmt(StatementType::IF);
   REQUIRE(retrievedIf.size() == 1);
   REQUIRE(*(retrievedIf.begin()) == ifStmt);

   // Print Statement
   set<shared_ptr<Statement>> retrievedPrint = queryService->getAllStmt(StatementType::PRINT);
   REQUIRE(retrievedPrint.size() == 1);
   REQUIRE(*(retrievedPrint.begin()) == printStmt);

   // Read Statement
   set<shared_ptr<Statement>> retrievedRead = queryService->getAllStmt(StatementType::READ);
   //REQUIRE(retrievedRead.size() == 1);
   //REQUIRE(*(retrievedRead.begin()) == readStmt);

   // While Statement
   set<shared_ptr<Statement>> retrievedWhile = queryService->getAllStmt(StatementType::WHILE);
   REQUIRE(retrievedWhile.size() == 1);
   REQUIRE(*(retrievedWhile.begin()) == whileStmt);


}