#include "catch.hpp"

using namespace std;

#include "PKB/PKB.h"
#include "PKB/Storage.h"
#include "PKB/QueryServicer.h"
#include "AST/SourceCode.h"
#include "AST/Expression/RelationalFactor/NameExpression.h"
#include "AST/Expression/RelationalFactor/ConstantExpression.h"
#include "SP/Parser.h"


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

TEST_CASE("QueryServicer - Procedure") {
    // Storage Stub
    shared_ptr<Storage> store = make_shared<Storage>();
    Procedure procMain = Procedure(nullptr, "main");
    Procedure procTest = Procedure(nullptr, "test");
    store->storeProc(procMain);
    store->storeProc(procTest);

    // Comparison Set
    set<Procedure> cmp_set;
    cmp_set.insert(procMain);
    cmp_set.insert(procTest);

    shared_ptr<QueryServicer> queryService = make_shared<QueryServicer>(store);
    REQUIRE(queryService->getAllProc() == cmp_set);
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

TEST_CASE("QueryServicer - Next Forward") {
    shared_ptr<PKB> pkb = make_shared<PKB>();

    string srcCode = "procedure main {\n"
                     "    while (flag >= 0) {\n"
                     "        if (flag >= 0) then {\n"
                     "            print flag;\n"
                     "        } else {\n"
                     "            read flag;\n"
                     "        }"
                     "        x = 5;"
                     "    }\n"
                     "    call abc;"
                     "}\n"
                     "procedure abc {\n"
                     "    while (flag > 0) {\n"
                     "        print flag;\n"
                     "        read flag;\n"
                     "    }\n"
                     "    print x;\n"
                     "}\n";
    Statement::resetLineNumCount();
    shared_ptr<SourceCode> sourceCode = Parser::parseSourceCode(srcCode, "");
    vector<shared_ptr<CFG> > cfgLst = sourceCode->getAllCFGs();

    shared_ptr<map<int, shared_ptr<CFGNode> >> cfgMap = sourceCode->getAllCFGMaps();
    pkb->buildFromCFG(cfgMap);

    vector<int> nextStatements = pkb->getQueryServicer()->forwardComputeRelation(1, NEXT);
    REQUIRE(nextStatements.size() == 2);

    vector<int> nextStarStatements = pkb->getQueryServicer()->forwardComputeRelation(1, NEXTS);
    REQUIRE(nextStarStatements.size() == 4);

    bool checkRetrieveRelation1 = pkb->getQueryServicer()->retrieveRelation(1, 2, NEXT);
    REQUIRE(checkRetrieveRelation1 == true);

    bool checkRetrieveRelation2 = pkb->getQueryServicer()->retrieveRelation(1, 3, NEXT);
    REQUIRE(checkRetrieveRelation2 == false);

    bool checkRetrieveRelation3 = pkb->getQueryServicer()->retrieveRelation(1, 2, NEXTS);
    REQUIRE(checkRetrieveRelation3 == true);

    bool checkRetrieveRelation4 = pkb->getQueryServicer()->retrieveRelation(1, 3, NEXTS);
    REQUIRE(checkRetrieveRelation4 == true);
}

TEST_CASE("QueryServicer - Next Backward") {
    shared_ptr<PKB> pkb = make_shared<PKB>();

    string srcCode = "procedure main {\n"
                     "    x = 5;"
                     "    while (flag >= 0) {\n"
                     "        if (flag >= 0) then {\n"
                     "            print flag;\n"
                     "        } else {\n"
                     "            read flag;\n"
                     "        }"
                     "    }\n"
                     "    call abc;"
                     "}\n";
    Statement::resetLineNumCount();
    shared_ptr<SourceCode> sourceCode = Parser::parseSourceCode(srcCode, "");
    vector<shared_ptr<CFG> > cfgLst = sourceCode->getAllCFGs();

    shared_ptr<map<int, shared_ptr<CFGNode> >> cfgMap = sourceCode->getAllCFGMaps();
    pkb->buildFromCFG(cfgMap);

    vector<int> nextStatements = pkb->getQueryServicer()->backwardComputeRelation(2, NEXT);
    // Contain lines 4,5,6
    REQUIRE(nextStatements.size() == 3);

    vector<int> nextStarStatements1 = pkb->getQueryServicer()->backwardComputeRelation(6, NEXTS);
    // Contain lines 1,2
    REQUIRE(nextStarStatements1.size() == 2);

    vector<int> nextStarStatements2 = pkb->getQueryServicer()->backwardComputeRelation(4, NEXTS);

    REQUIRE(nextStarStatements2.size() == 3);
}

TEST_CASE("QueryServicer - Next Backward While Statement") {
    shared_ptr<PKB> pkb = make_shared<PKB>();

    string srcCode = "procedure main {\n"
                     "    while (flag >= 0) {\n"
                     "        if (flag >= 0) then {\n"
                     "            print flag;\n"
                     "        } else {\n"
                     "            read flag;\n"
                     "        }"
                     "        x = 5;"
                     "    }\n"
                     "    call abc;"
                     "}\n"
                     "procedure abc {\n"
                     "    while (flag > 0) {\n"
                     "        print flag;\n"
                     "        read flag;\n"
                     "    }\n"
                     "    print x;\n"
                     "}\n";
    Statement::resetLineNumCount();
    shared_ptr<SourceCode> sourceCode = Parser::parseSourceCode(srcCode, "");
    vector<shared_ptr<CFG> > cfgLst = sourceCode->getAllCFGs();

    shared_ptr<map<int, shared_ptr<CFGNode> >> cfgMap = sourceCode->getAllCFGMaps();
    pkb->buildFromCFG(cfgMap);

    vector<int> nextStatements = pkb->getQueryServicer()->backwardComputeRelation(1, NEXT);
    REQUIRE(nextStatements.size() == 1);

    vector<int> nextStarStatements = pkb->getQueryServicer()->backwardComputeRelation(1, NEXTS);
    REQUIRE(nextStarStatements.size() == 1);
}

TEST_CASE("FAILING TESTCASE") {
    shared_ptr<PKB> pkb = make_shared<PKB>();

    string srcCode = "procedure main {\n"
                     "    while (flag >= 0) {\n"
                     "        if (flag >= 0) then {\n"
                     "            print flag;\n"
                     "        } else {\n"
                     "            read flag;\n"
                     "        }"
                     "        x = 5;"
                     "    }\n"
                     "    call abc;"
                     "}\n"
                     "procedure abc {\n"
                     "    while (flag > 0) {\n"
                     "        print flag;\n"
                     "        read flag;\n"
                     "    }\n"
                     "    print x;\n"
                     "}\n";
    Statement::resetLineNumCount();
    shared_ptr<SourceCode> sourceCode = Parser::parseSourceCode(srcCode, "");
    vector<shared_ptr<CFG> > cfgLst = sourceCode->getAllCFGs();

    shared_ptr<map<int, shared_ptr<CFGNode> >> cfgMap = sourceCode->getAllCFGMaps();
    pkb->buildFromCFG(cfgMap);


    shared_ptr<CFGNode> cfgNode = cfgMap->at(5);


    bool checkRetrieveRelation1 = pkb->getQueryServicer()->retrieveRelation(3, 5, NEXT);
    REQUIRE(checkRetrieveRelation1 == true);

    bool checkRetrieveRelation2 = pkb->getQueryServicer()->retrieveRelation(4, 5, NEXT);
    REQUIRE(checkRetrieveRelation2 == true);

}