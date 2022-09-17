#include "catch.hpp"

using namespace std;

#include "PKB/Storage.h"
#include "PKB/QueryServicer.h"
#include "AST/SourceCode.h"
#include "AST/Expression/RelationalFactor/NameExpression.h"
#include "AST/Expression/RelationalFactor/ConstantExpression.h"


void require(bool b);



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
    require(queryService->getAllVar() == cmp_set);
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
    require(queryService->getAllConst() == cmp_set);
}

TEST_CASE("QueryServicer - Statements") {
    // Storage Stub
    shared_ptr<Storage> store = make_shared<Storage>();

    shared_ptr<AssignStatement> assignStmt = make_shared<AssignStatement>(nullptr, "x", nullptr);
    shared_ptr<CallStatement> callStmt = make_shared<CallStatement>(nullptr, "procedure_x");
    shared_ptr<IfStatement> ifStmt = make_shared<IfStatement>(nullptr, nullptr);
    shared_ptr<PrintStatement> printStmt = make_shared<PrintStatement>(nullptr, "x");
    shared_ptr<ReadStatement> readStmt = make_shared<ReadStatement>(nullptr, "x");
    shared_ptr<WhileStatement> whileStmt = make_shared<WhileStatement>(nullptr, nullptr);

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
    require(queryService->getAllStmt(StatementType::STATEMENT) == cmp_set);

    // Assign Statement
   set<shared_ptr<Statement>> retrievedAssign = queryService->getAllStmt(StatementType::ASSIGN);
   require(retrievedAssign.size() == 1);
   require(*(retrievedAssign.begin()) == assignStmt);

   // Call Statement
   set<shared_ptr<Statement>> retrievedCall = queryService->getAllStmt(StatementType::CALL);
   /*require(retrievedCall.size() == 1);*/
   //require(*(retrievedCall.begin()) == callStmt);

   // If Statement
   set<shared_ptr<Statement>> retrievedIf = queryService->getAllStmt(StatementType::IF);
   require(retrievedIf.size() == 1);
   require(*(retrievedIf.begin()) == ifStmt);

   // Print Statement
   set<shared_ptr<Statement>> retrievedPrint = queryService->getAllStmt(StatementType::PRINT);
   require(retrievedPrint.size() == 1);
   require(*(retrievedPrint.begin()) == printStmt);

   // Read Statement
   set<shared_ptr<Statement>> retrievedRead = queryService->getAllStmt(StatementType::READ);
   //require(retrievedRead.size() == 1);
   //require(*(retrievedRead.begin()) == readStmt);

   // While Statement
   set<shared_ptr<Statement>> retrievedWhile = queryService->getAllStmt(StatementType::WHILE);
   require(retrievedWhile.size() == 1);
   require(*(retrievedWhile.begin()) == whileStmt);


}