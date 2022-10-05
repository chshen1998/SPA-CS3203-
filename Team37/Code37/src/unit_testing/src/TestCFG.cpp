#include "AST/Statement/ReadStatement.h"
#include "AST/Statement/PrintStatement.h"
#include "AST/Statement/CallStatement.h"
#include "AST/Statement/WhileStatement.h"
#include "AST/Statement/IfStatement.h"
#include "AST/Statement/AssignStatement.h"
#include "AST/Expression/ConditionalExpression/RelationalExpression.h"
#include "AST/Expression/RelationalFactor/ConstantExpression.h"
#include "AST/Expression/RelationalFactor/NameExpression.h"
#include "AST/Operators/RelationalOperator.h"
#include "SP/Parser.h"

#include "catch.hpp"

using namespace std;

TEST_CASE("Simple While statement") {
    shared_ptr<ConditionalExpression> expr = make_shared<RelationalExpression>(
            nullptr,
            RelationalOperator::EQUALS,
            make_shared<ConstantExpression>(nullptr, 10),
            make_shared<ConstantExpression>(nullptr, 11)
    );
    shared_ptr<WhileStatement> whileStatement = make_shared<WhileStatement>(nullptr, expr);
    shared_ptr<ReadStatement> readStatement = make_shared<ReadStatement>(whileStatement, "x");
    shared_ptr<PrintStatement> printStatement = make_shared<PrintStatement>(whileStatement, "y");
    readStatement->setParent(whileStatement);
    printStatement->setParent(whileStatement);
    whileStatement->addStatement(readStatement);
    whileStatement->addStatement(printStatement);
    vector<shared_ptr<CFGNode> > parents;
    shared_ptr<CFGNode> dummyNode = whileStatement->buildCFG(parents);

    REQUIRE(dummyNode->getParents().size() == 2);
    shared_ptr<CFGNode> cfgNode1 = dummyNode->getParents()[1];
    REQUIRE(cfgNode1->getTNode() == whileStatement);
    REQUIRE(cfgNode1->getParents().size() == 1);
    REQUIRE(cfgNode1->getNumChildren() == 2);
    shared_ptr<CFGNode> cfgNode2 = cfgNode1->getChild(0);
    REQUIRE(cfgNode2->getTNode() == readStatement);
    REQUIRE(cfgNode2->getNumChildren() == 1);
    shared_ptr<CFGNode> cfgNode3 = cfgNode2->getChild(0);
    REQUIRE(cfgNode3->getTNode() == printStatement);
    REQUIRE(cfgNode3->getNumChildren() == 2);
    REQUIRE(cfgNode3->getChild(0)->getTNode() == whileStatement);
    REQUIRE(cfgNode3->getChild(1)->getTNode() == nullptr);
    REQUIRE(dummyNode->getStoredStmt() == cfgNode1);
}

TEST_CASE("Nested While statement") {
    shared_ptr<ConditionalExpression> expr = make_shared<RelationalExpression>(
            nullptr,
            RelationalOperator::EQUALS,
            make_shared<ConstantExpression>(nullptr, 10),
            make_shared<ConstantExpression>(nullptr, 11)
    );
    shared_ptr<WhileStatement> whileStatement = make_shared<WhileStatement>(nullptr, expr);
    shared_ptr<ReadStatement> readStatement = make_shared<ReadStatement>(whileStatement, "x");
    shared_ptr<WhileStatement> whileStatement2 = make_shared<WhileStatement>(whileStatement, expr);
    shared_ptr<PrintStatement> printStatement = make_shared<PrintStatement>(whileStatement, "y");
    readStatement->setParent(whileStatement);
    whileStatement->addStatement(readStatement);
    whileStatement->addStatement(whileStatement2);
    printStatement->setParent(whileStatement2);
    whileStatement2->addStatement(printStatement);
    vector<shared_ptr<CFGNode> > parents;
    shared_ptr<CFGNode> dummyNode2 = whileStatement->buildCFG(parents);

    REQUIRE(dummyNode2->getParents().size() == 2);
    shared_ptr<CFGNode> cfgNode1 = dummyNode2->getParents()[1];
    REQUIRE(cfgNode1->getTNode() == whileStatement);
    REQUIRE(cfgNode1->getParents().size() == 1);
    REQUIRE(cfgNode1->getNumChildren() == 2);
    shared_ptr<CFGNode> cfgNode2 = cfgNode1->getChild(0);
    REQUIRE(cfgNode2->getTNode() == readStatement);
    REQUIRE(cfgNode2->getParents().size() == 1);
    REQUIRE(cfgNode2->getNumChildren() == 1);
    shared_ptr<CFGNode> cfgNode3 = cfgNode2->getChild(0);
    REQUIRE(cfgNode3->getTNode() == whileStatement2);
    REQUIRE(cfgNode3->getParents().size() == 2);
    REQUIRE(cfgNode3->getNumChildren() == 2);
    shared_ptr<CFGNode> cfgNode4 = cfgNode3->getChild(0);
    REQUIRE(cfgNode4->getTNode() == printStatement);
    REQUIRE(cfgNode4->getChild(0)->getTNode() == whileStatement2);
    shared_ptr<CFGNode> dummyNode1 = cfgNode4->getChild(1);
    REQUIRE(dummyNode1->getTNode() == nullptr);
    REQUIRE(cfgNode3->getChild(1)->getTNode() == nullptr);
    dummyNode2 = dummyNode1->getChild(1);
    REQUIRE(dummyNode2->getTNode() == nullptr);
    REQUIRE(dummyNode1->getChild(0)->getTNode() == whileStatement);
    REQUIRE(dummyNode2->getParents()[1]->getTNode() == whileStatement);
}

TEST_CASE("Simple If Statement") {
    shared_ptr<ConditionalExpression> expr = make_shared<RelationalExpression>(
            nullptr,
            RelationalOperator::EQUALS,
            make_shared<ConstantExpression>(nullptr, 10),
            make_shared<ConstantExpression>(nullptr, 11)
    );
    shared_ptr<IfStatement> ifStatement = make_shared<IfStatement>(nullptr, expr);
    shared_ptr<ReadStatement> readStatement = make_shared<ReadStatement>(nullptr, "x");
    shared_ptr<PrintStatement> printStatement = make_shared<PrintStatement>(nullptr, "y");
    readStatement->setParent(ifStatement);
    printStatement->setParent(ifStatement);
    ifStatement->addThenStatement(readStatement);
    ifStatement->addElseStatement(printStatement);

    vector<shared_ptr<CFGNode> > parents;
    shared_ptr<CFGNode> result = ifStatement->buildCFG(parents);
    REQUIRE(result->getParents().size() == 2);
    shared_ptr<CFGNode> cfgNode2 = result->getParents()[0];
    REQUIRE(cfgNode2->getTNode() == readStatement);
    REQUIRE(cfgNode2->getParents().size() == 1);
    REQUIRE(cfgNode2->getNumChildren() == 1);
    shared_ptr<CFGNode> cfgNode1 = cfgNode2->getParents()[0];
    REQUIRE(cfgNode1->getTNode() == ifStatement);
    shared_ptr<CFGNode> cfgNode3 = result->getParents()[1];
    REQUIRE(cfgNode3->getTNode() == printStatement);
    REQUIRE(cfgNode3->getParents().size() == 1);
    REQUIRE(cfgNode3->getNumChildren() == 1);
    cfgNode1 = cfgNode3->getParents()[0];
    REQUIRE(cfgNode1->getTNode() == ifStatement);
    REQUIRE(result->getStoredStmt() == cfgNode1);
}

TEST_CASE("Nested If statements") {
    shared_ptr<ConditionalExpression> expr = make_shared<RelationalExpression>(
            nullptr,
            RelationalOperator::EQUALS,
            make_shared<ConstantExpression>(nullptr, 10),
            make_shared<ConstantExpression>(nullptr, 11)
    );
    shared_ptr<IfStatement> ifStatement = make_shared<IfStatement>(nullptr, expr);
    shared_ptr<ReadStatement> readStatement = make_shared<ReadStatement>(nullptr, "x");
    readStatement->setParent(ifStatement);
    ifStatement->addThenStatement(readStatement);

    shared_ptr<IfStatement> ifStatement2 = make_shared<IfStatement>(nullptr, expr);
    ifStatement2->setParent(ifStatement);
    ifStatement->addElseStatement(ifStatement2);

    shared_ptr<PrintStatement> printStatement = make_shared<PrintStatement>(nullptr, "y");
    printStatement->setParent(ifStatement2);
    ifStatement2->addThenStatement(printStatement);

    shared_ptr<AssignStatement> assignStatement =
            make_shared<AssignStatement>(nullptr, "x", make_shared<NameExpression>(nullptr, "y"));
    assignStatement->setParent(ifStatement2);
    ifStatement2->addElseStatement(assignStatement);

    vector<shared_ptr<CFGNode> > parents;
    shared_ptr<CFGNode> dummyNode1 = ifStatement->buildCFG(parents);
    REQUIRE(dummyNode1->getParents().size() == 2);
    shared_ptr<CFGNode> cfgNode2 = dummyNode1->getParents()[0];
    REQUIRE(cfgNode2->getTNode() == readStatement);
    REQUIRE(cfgNode2->getParents().size() == 1);
    shared_ptr<CFGNode> cfgNode1 = cfgNode2->getParents()[0];
    REQUIRE(cfgNode1->getTNode() == ifStatement);
    shared_ptr<CFGNode> dummyNode2 = dummyNode1->getParents()[1];
    REQUIRE(dummyNode2->getParents().size() == 2);
    shared_ptr<CFGNode> cfgNode4 = dummyNode2->getParents()[0];
    REQUIRE(cfgNode4->getTNode() == printStatement);
    REQUIRE(cfgNode4->getParents().size() == 1);
    shared_ptr<CFGNode> cfgNode3 = cfgNode4->getParents()[0];
    REQUIRE(cfgNode3->getTNode() == ifStatement2);
    shared_ptr<CFGNode> cfgNode5 = dummyNode2->getParents()[1];
    REQUIRE(cfgNode5->getTNode() == assignStatement);
    REQUIRE(cfgNode5->getParents().size() == 1);
    cfgNode3 = cfgNode5->getParents()[0];
    REQUIRE(cfgNode3->getTNode() == ifStatement2);
    REQUIRE(cfgNode3->getParents().size() == 1);
    REQUIRE(cfgNode3->getParents()[0]->getTNode() == ifStatement);

    REQUIRE(dummyNode1->getStoredStmt() == cfgNode1);
    REQUIRE(dummyNode2->getStoredStmt() == cfgNode3);
}

TEST_CASE("Procedure with while statement") {
    string proc = "procedure main {\n"
                "    while (flag > 0) {\n"
                "        print flag;\n"
                "        read flag;\n"
                "    }\n"
                "    print x;\n"
                "}\n";
    shared_ptr<Procedure> procedure = Parser::parseProcedure(proc);
    procedure->buildCFG(procedure->getProcedureName());
    shared_ptr<CFG> cfg = procedure->getCFG();
    REQUIRE(cfg->getName() == "main");
    shared_ptr<CFGNode> cfgNode1 = cfg->getStartNode();
    REQUIRE(cfgNode1->getNumChildren() == 2);
    shared_ptr<WhileStatement> whileStatement = dynamic_pointer_cast<WhileStatement>(cfgNode1->getTNode());
    shared_ptr<RelationalExpression> condExpr =
            dynamic_pointer_cast<RelationalExpression>(whileStatement->getConditionalExpression());
    shared_ptr<NameExpression> nameExpr = dynamic_pointer_cast<NameExpression>(condExpr->getRelFactor1());
    shared_ptr<ConstantExpression> constantExpression =
            dynamic_pointer_cast<ConstantExpression>(condExpr->getRelFactor2());
    REQUIRE(nameExpr->getVarName() == "flag");
    REQUIRE(constantExpression->getValue() == 0);
    REQUIRE(condExpr->getOperator() == RelationalOperator::GREATER_THAN);
    shared_ptr<CFGNode> cfgNode2 = cfgNode1->getChild(0);
    shared_ptr<PrintStatement> printStatement = dynamic_pointer_cast<PrintStatement>(cfgNode2->getTNode());
    REQUIRE(printStatement->getVariableName() == "flag");
    shared_ptr<CFGNode> dummyNode = cfgNode1->getChild(1);
    REQUIRE(dummyNode->getTNode() == nullptr);
    REQUIRE(dummyNode->getStoredStmt() == cfgNode1);
    REQUIRE(dummyNode->getNumChildren() == 1);
    shared_ptr<CFGNode> cfgNode4 = dummyNode->getChild(0);
    shared_ptr<PrintStatement> printStatement2 = dynamic_pointer_cast<PrintStatement>(cfgNode4->getTNode());
    REQUIRE(printStatement2->getVariableName() == "x");
    REQUIRE(cfgNode2->getNumChildren() == 1);
    shared_ptr<CFGNode> cfgNode3 = cfgNode2->getChild(0);
    shared_ptr<ReadStatement> readStatement = dynamic_pointer_cast<ReadStatement>(cfgNode3->getTNode());
    REQUIRE(readStatement->getVariableName() == "flag");
    REQUIRE(cfgNode3->getNumChildren() == 2);
    cfgNode1 = cfgNode3->getChild(0);
    whileStatement = dynamic_pointer_cast<WhileStatement>(cfgNode1->getTNode());
    condExpr = dynamic_pointer_cast<RelationalExpression>(whileStatement->getConditionalExpression());
    nameExpr = dynamic_pointer_cast<NameExpression>(condExpr->getRelFactor1());
    constantExpression = dynamic_pointer_cast<ConstantExpression>(condExpr->getRelFactor2());
    REQUIRE(nameExpr->getVarName() == "flag");
    REQUIRE(constantExpression->getValue() == 0);
    REQUIRE(condExpr->getOperator() == RelationalOperator::GREATER_THAN);
}

TEST_CASE("Procedure with while statement and if statement") {
    string proc = "procedure main {\n"
                     "    while (flag >= 0) {\n"
                     "        if (flag >= 0) then {\n"
                     "            print flag;\n"
                     "        } else {\n"
                     "            read flag;\n"
                     "        }"
                     "        x = 5;"
                     "    }\n"
                     "        flag = 5;"
                     "}\n";
    shared_ptr<Procedure> procedure = Parser::parseProcedure(proc);
    procedure->buildCFG(procedure->getProcedureName());
    shared_ptr<CFG> cfg = procedure->getCFG();
    REQUIRE(cfg->getName() == "main");

    shared_ptr<CFGNode> cfgNode1 = cfg->getStartNode();
    REQUIRE(cfgNode1->getNumChildren() == 2);
    shared_ptr<WhileStatement> whileStatement = dynamic_pointer_cast<WhileStatement>(cfgNode1->getTNode());
    shared_ptr<RelationalExpression> condExpr =
            dynamic_pointer_cast<RelationalExpression>(whileStatement->getConditionalExpression());
    shared_ptr<NameExpression> nameExpr = dynamic_pointer_cast<NameExpression>(condExpr->getRelFactor1());
    shared_ptr<ConstantExpression> constantExpression =
            dynamic_pointer_cast<ConstantExpression>(condExpr->getRelFactor2());
    REQUIRE(nameExpr->getVarName() == "flag");
    REQUIRE(constantExpression->getValue() == 0);
    REQUIRE(condExpr->getOperator() == RelationalOperator::GREATER_THAN_OR_EQUALS);
    REQUIRE(cfgNode1->getNumChildren() == 2);
    shared_ptr<CFGNode> cfgNode2 = cfgNode1->getChild(0);
    shared_ptr<CFGNode> dummyNode2 = cfgNode1->getChild(1);
    REQUIRE(dummyNode2->getTNode() == nullptr);
    REQUIRE(dummyNode2->getStoredStmt() == cfgNode1);
    REQUIRE(dummyNode2->getNumChildren() == 1);
    shared_ptr<IfStatement> ifStatement = dynamic_pointer_cast<IfStatement>(cfgNode2->getTNode());
    shared_ptr<RelationalExpression> condExpr2 =
            dynamic_pointer_cast<RelationalExpression>(ifStatement->getConditionalExpression());
    shared_ptr<NameExpression> nameExpr2 = dynamic_pointer_cast<NameExpression>(condExpr2->getRelFactor1());
    shared_ptr<ConstantExpression> constantExpression2 =
            dynamic_pointer_cast<ConstantExpression>(condExpr2->getRelFactor2());
    REQUIRE(nameExpr2->getVarName() == "flag");
    REQUIRE(constantExpression2->getValue() == 0);
    REQUIRE(condExpr2->getOperator() == RelationalOperator::GREATER_THAN_OR_EQUALS);
    REQUIRE(cfgNode2->getNumChildren() == 2);
    shared_ptr<CFGNode> cfgNode3 = cfgNode2->getChild(0);
    shared_ptr<PrintStatement> printStatement = dynamic_pointer_cast<PrintStatement>(cfgNode3->getTNode());
    REQUIRE(printStatement->getVariableName() == "flag");
    shared_ptr<CFGNode> dummyNode1 = cfgNode3->getChild(0);
    REQUIRE(dummyNode1->getNumChildren() == 1);
    shared_ptr<CFGNode> cfgNode4 = cfgNode2->getChild(1);
    REQUIRE(cfgNode4->getNumChildren() == 1);
    dummyNode1 = cfgNode4->getChild(0);
    REQUIRE(dummyNode1->getTNode() == nullptr);
    REQUIRE(dummyNode1->getStoredStmt() == cfgNode2);
    shared_ptr<CFGNode> cfgNode5 = dummyNode1->getChild(0);
    shared_ptr<AssignStatement> assignStatement = dynamic_pointer_cast<AssignStatement>(cfgNode5->getTNode());
    REQUIRE(assignStatement->getVarName() == "x");
    shared_ptr<ConstantExpression> constantExpression3 =
            dynamic_pointer_cast<ConstantExpression>(assignStatement->getRelFactor());
    REQUIRE(constantExpression3->getValue() == 5);
    REQUIRE(cfgNode5->getNumChildren() == 2);
    REQUIRE(cfgNode5->getChild(0) == cfgNode1);
    REQUIRE(cfgNode5->getChild(1) == dummyNode2);
    REQUIRE(dummyNode2->getNumChildren() == 1);
    shared_ptr<CFGNode> cfgNode6 = dummyNode2->getChild(0);
    assignStatement = dynamic_pointer_cast<AssignStatement>(cfgNode6->getTNode());
    REQUIRE(assignStatement->getVarName() == "flag");
    constantExpression3 =
            dynamic_pointer_cast<ConstantExpression>(assignStatement->getRelFactor());
    REQUIRE(constantExpression3->getValue() == 5);
    REQUIRE(cfgNode6->getNumChildren() == 0);
}

TEST_CASE("Sourcecode with two procedures") {
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
    shared_ptr<SourceCode> sourceCode = Parser::parseSourceCode(srcCode, "");
    vector<shared_ptr<CFG> > cfgLst = sourceCode->getAllCFGs();
    REQUIRE(cfgLst.size() == 2);

    shared_ptr<CFG> cfg1 = cfgLst[0];
    REQUIRE(cfg1->getName() == "main");
    shared_ptr<CFGNode> cfgNode1 = cfg1->getStartNode();
    REQUIRE(cfgNode1->getNumChildren() == 2);
    shared_ptr<WhileStatement> whileStatement = dynamic_pointer_cast<WhileStatement>(cfgNode1->getTNode());
    shared_ptr<RelationalExpression> condExpr =
            dynamic_pointer_cast<RelationalExpression>(whileStatement->getConditionalExpression());
    shared_ptr<NameExpression> nameExpr = dynamic_pointer_cast<NameExpression>(condExpr->getRelFactor1());
    shared_ptr<ConstantExpression> constantExpression =
            dynamic_pointer_cast<ConstantExpression>(condExpr->getRelFactor2());
    REQUIRE(nameExpr->getVarName() == "flag");
    REQUIRE(constantExpression->getValue() == 0);
    REQUIRE(condExpr->getOperator() == RelationalOperator::GREATER_THAN_OR_EQUALS);
    REQUIRE(cfgNode1->getNumChildren() == 2);
    shared_ptr<CFGNode> cfgNode2 = cfgNode1->getChild(0);
    shared_ptr<CFGNode> dummyNode2 = cfgNode1->getChild(1);
    REQUIRE(dummyNode2->getTNode() == nullptr);
    REQUIRE(dummyNode2->getStoredStmt() == cfgNode1);
    REQUIRE(dummyNode2->getNumChildren() == 1);
    shared_ptr<IfStatement> ifStatement = dynamic_pointer_cast<IfStatement>(cfgNode2->getTNode());
    shared_ptr<RelationalExpression> condExpr2 =
            dynamic_pointer_cast<RelationalExpression>(ifStatement->getConditionalExpression());
    shared_ptr<NameExpression> nameExpr2 = dynamic_pointer_cast<NameExpression>(condExpr2->getRelFactor1());
    shared_ptr<ConstantExpression> constantExpression2 =
            dynamic_pointer_cast<ConstantExpression>(condExpr2->getRelFactor2());
    REQUIRE(nameExpr2->getVarName() == "flag");
    REQUIRE(constantExpression2->getValue() == 0);
    REQUIRE(condExpr2->getOperator() == RelationalOperator::GREATER_THAN_OR_EQUALS);
    REQUIRE(cfgNode2->getNumChildren() == 2);
    shared_ptr<CFGNode> cfgNode3 = cfgNode2->getChild(0);
    shared_ptr<PrintStatement> printStatement = dynamic_pointer_cast<PrintStatement>(cfgNode3->getTNode());
    REQUIRE(printStatement->getVariableName() == "flag");
    shared_ptr<CFGNode> dummyNode1 = cfgNode3->getChild(0);
    REQUIRE(dummyNode1->getNumChildren() == 1);
    shared_ptr<CFGNode> cfgNode4 = cfgNode2->getChild(1);
    REQUIRE(cfgNode4->getNumChildren() == 1);
    dummyNode1 = cfgNode4->getChild(0);
    REQUIRE(dummyNode1->getTNode() == nullptr);
    REQUIRE(dummyNode1->getStoredStmt() == cfgNode2);
    shared_ptr<CFGNode> cfgNode5 = dummyNode1->getChild(0);
    shared_ptr<AssignStatement> assignStatement = dynamic_pointer_cast<AssignStatement>(cfgNode5->getTNode());
    REQUIRE(assignStatement->getVarName() == "x");
    shared_ptr<ConstantExpression> constantExpression3 =
            dynamic_pointer_cast<ConstantExpression>(assignStatement->getRelFactor());
    REQUIRE(constantExpression3->getValue() == 5);
    REQUIRE(cfgNode5->getNumChildren() == 2);
    REQUIRE(cfgNode5->getChild(0) == cfgNode1);
    REQUIRE(cfgNode5->getChild(1) == dummyNode2);
    REQUIRE(dummyNode2->getNumChildren() == 1);
    shared_ptr<CFGNode> cfgNode6 = dummyNode2->getChild(0);
    shared_ptr<CallStatement> callStatement = dynamic_pointer_cast<CallStatement>(cfgNode6->getTNode());
    REQUIRE(callStatement->getProcedureName() == "abc");
    REQUIRE(cfgNode6->getNumChildren() == 0);

    shared_ptr<CFG> cfg2 = cfgLst[1];
    REQUIRE(cfg2->getName() == "abc");
    cfgNode1 = cfg2->getStartNode();
    REQUIRE(cfgNode1->getNumChildren() == 2);
    whileStatement = dynamic_pointer_cast<WhileStatement>(cfgNode1->getTNode());
    condExpr = dynamic_pointer_cast<RelationalExpression>(whileStatement->getConditionalExpression());
    nameExpr = dynamic_pointer_cast<NameExpression>(condExpr->getRelFactor1());
    constantExpression = dynamic_pointer_cast<ConstantExpression>(condExpr->getRelFactor2());
    REQUIRE(nameExpr->getVarName() == "flag");
    REQUIRE(constantExpression->getValue() == 0);
    REQUIRE(condExpr->getOperator() == RelationalOperator::GREATER_THAN);
    cfgNode2 = cfgNode1->getChild(0);
    printStatement = dynamic_pointer_cast<PrintStatement>(cfgNode2->getTNode());
    REQUIRE(printStatement->getVariableName() == "flag");
    shared_ptr<CFGNode> dummyNode = cfgNode1->getChild(1);
    REQUIRE(dummyNode->getTNode() == nullptr);
    REQUIRE(dummyNode->getStoredStmt() == cfgNode1);
    REQUIRE(dummyNode->getNumChildren() == 1);
    cfgNode4 = dummyNode->getChild(0);
    shared_ptr<PrintStatement> printStatement2 = dynamic_pointer_cast<PrintStatement>(cfgNode4->getTNode());
    REQUIRE(printStatement2->getVariableName() == "x");
    REQUIRE(cfgNode2->getNumChildren() == 1);
    cfgNode3 = cfgNode2->getChild(0);
    shared_ptr<ReadStatement> readStatement = dynamic_pointer_cast<ReadStatement>(cfgNode3->getTNode());
    REQUIRE(readStatement->getVariableName() == "flag");
    REQUIRE(cfgNode3->getNumChildren() == 2);
    cfgNode1 = cfgNode3->getChild(0);
    whileStatement = dynamic_pointer_cast<WhileStatement>(cfgNode1->getTNode());
    condExpr = dynamic_pointer_cast<RelationalExpression>(whileStatement->getConditionalExpression());
    nameExpr = dynamic_pointer_cast<NameExpression>(condExpr->getRelFactor1());
    constantExpression = dynamic_pointer_cast<ConstantExpression>(condExpr->getRelFactor2());
    REQUIRE(nameExpr->getVarName() == "flag");
    REQUIRE(constantExpression->getValue() == 0);
    REQUIRE(condExpr->getOperator() == RelationalOperator::GREATER_THAN);
}