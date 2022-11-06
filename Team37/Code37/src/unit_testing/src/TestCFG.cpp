#include "AST/Statement/ReadStatement.h"
#include "AST/Statement/PrintStatement.h"
#include "AST/Statement/CallStatement.h"
#include "AST/Statement/WhileStatement.h"
#include "AST/Statement/IfStatement.h"
#include "AST/Statement/AssignStatement.h"
#include "AST/Expression/ConditionalExpression/RelationalExpression.h"
#include "AST/Expression/RelationalFactor/ConstantExpression.h"
#include "AST/Expression/RelationalFactor/NameExpression.h"
#include "AST/Expression/RelationalFactor/OperatedExpression.h"
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
    shared_ptr<CFG> cfg = make_shared<CFG>(nullptr, "");
    shared_ptr<CFGNode> cfgNode1 = whileStatement->buildCFG(parents, cfg);

    REQUIRE(cfgNode1->getTNode() == whileStatement);
    REQUIRE(cfgNode1->getParents().size() == 1);
    REQUIRE(cfgNode1->getNumChildren() == 1);
    shared_ptr<CFGNode> cfgNode2 = cfgNode1->getChild(0);
    REQUIRE(cfgNode2->getTNode() == readStatement);
    REQUIRE(cfgNode2->getNumChildren() == 1);
    shared_ptr<CFGNode> cfgNode3 = cfgNode2->getChild(0);
    REQUIRE(cfgNode3->getTNode() == printStatement);
    REQUIRE(cfgNode3->getNumChildren() == 1);
    REQUIRE(cfgNode3->getChild(0)->getTNode() == whileStatement);
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
    shared_ptr<CFG> cfg = make_shared<CFG>(nullptr, "");
    shared_ptr<CFGNode> cfgNode1 = whileStatement->buildCFG(parents, cfg);

    REQUIRE(cfgNode1->getTNode() == whileStatement);
    REQUIRE(cfgNode1->getParents().size() == 1);
    REQUIRE(cfgNode1->getNumChildren() == 1);
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
    REQUIRE(cfgNode3->getChild(1)->getTNode() == whileStatement);
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
    shared_ptr<CFG> cfg = make_shared<CFG>(nullptr, "");
    shared_ptr<CFGNode> result = ifStatement->buildCFG(parents, cfg);
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
    shared_ptr<CFG> cfg = make_shared<CFG>(nullptr, "");
    shared_ptr<CFGNode> dummyNode1 = ifStatement->buildCFG(parents, cfg);
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
    shared_ptr<CFGNode> cfgNode4 = cfgNode1->getChild(1);
    REQUIRE(cfgNode4->getNumChildren() == 0);
    REQUIRE(cfgNode4->getParents().size() == 1);
    shared_ptr<PrintStatement> printStatement2 = dynamic_pointer_cast<PrintStatement>(cfgNode4->getTNode());
    REQUIRE(printStatement2->getVariableName() == "x");
    REQUIRE(cfgNode2->getNumChildren() == 1);
    shared_ptr<CFGNode> cfgNode3 = cfgNode2->getChild(0);
    shared_ptr<ReadStatement> readStatement = dynamic_pointer_cast<ReadStatement>(cfgNode3->getTNode());
    REQUIRE(readStatement->getVariableName() == "flag");
    REQUIRE(cfgNode3->getNumChildren() == 1);
    cfgNode1 = cfgNode3->getChild(0);
    whileStatement = dynamic_pointer_cast<WhileStatement>(cfgNode1->getTNode());
    condExpr = dynamic_pointer_cast<RelationalExpression>(whileStatement->getConditionalExpression());
    nameExpr = dynamic_pointer_cast<NameExpression>(condExpr->getRelFactor1());
    constantExpression = dynamic_pointer_cast<ConstantExpression>(condExpr->getRelFactor2());
    REQUIRE(nameExpr->getVarName() == "flag");
    REQUIRE(constantExpression->getValue() == 0);
    REQUIRE(condExpr->getOperator() == RelationalOperator::GREATER_THAN);

    REQUIRE(cfg->getMap()->size() == 4);
}

TEST_CASE("Procedure with while statement 2") {
    string proc = "procedure main {\n"
                  "while(i == 11) {\n"
                  "    flag = 0;\n"
                  "    print flag;\n"
                  "    read flag;\n"
                  "    x = x + 2 + 0 + flag;}\n"
                  "}\n";
    shared_ptr<Procedure> procedure = Parser::parseProcedure(proc);
    procedure->buildCFG(procedure->getProcedureName());
    shared_ptr<CFG> cfg = procedure->getCFG();
    REQUIRE(cfg->getName() == "main");

    shared_ptr<CFGNode> cfgNode1 = cfg->getStartNode();
    REQUIRE(cfgNode1->getNumChildren() == 1);
    shared_ptr<WhileStatement> whileStatement = dynamic_pointer_cast<WhileStatement>(cfgNode1->getTNode());
    shared_ptr<RelationalExpression> condExpr =
            dynamic_pointer_cast<RelationalExpression>(whileStatement->getConditionalExpression());
    shared_ptr<NameExpression> nameExpr = dynamic_pointer_cast<NameExpression>(condExpr->getRelFactor1());
    shared_ptr<ConstantExpression> constantExpression =
            dynamic_pointer_cast<ConstantExpression>(condExpr->getRelFactor2());
    REQUIRE(nameExpr->getVarName() == "i");
    REQUIRE(constantExpression->getValue() == 11);
    REQUIRE(condExpr->getOperator() == RelationalOperator::EQUALS);

    shared_ptr<CFGNode> cfgNode2 = cfgNode1->getChild(0);
    shared_ptr<AssignStatement> assignStatement1 = dynamic_pointer_cast<AssignStatement>(cfgNode2->getTNode());
    REQUIRE(assignStatement1->getVarName() == "flag");
    shared_ptr<ConstantExpression> constExpr = dynamic_pointer_cast<ConstantExpression>(assignStatement1->getRelFactor());
    REQUIRE(constExpr->getValue() == 0);
    REQUIRE(cfgNode2->getNumChildren() == 1);

    shared_ptr<CFGNode> cfgNode3 = cfgNode2->getChild(0);
    shared_ptr<PrintStatement> printStatement = dynamic_pointer_cast<PrintStatement>(cfgNode3->getTNode());
    REQUIRE(printStatement->getVariableName() == "flag");
    REQUIRE(cfgNode3->getNumChildren() == 1);

    shared_ptr<CFGNode> cfgNode4 = cfgNode3->getChild(0);
    shared_ptr<ReadStatement> readStatement = dynamic_pointer_cast<ReadStatement>(cfgNode4->getTNode());
    REQUIRE(readStatement->getVariableName() == "flag");
    REQUIRE(cfgNode4->getNumChildren() == 1);

    shared_ptr<CFGNode> cfgNode5 = cfgNode4->getChild(0);
    shared_ptr<AssignStatement> assignStatement2 = dynamic_pointer_cast<AssignStatement>(cfgNode5->getTNode());
    REQUIRE(assignStatement2->getVarName() == "x");
    shared_ptr<OperatedExpression> opExpr = dynamic_pointer_cast<OperatedExpression>(assignStatement1->getRelFactor());
    REQUIRE(cfgNode5->getNumChildren() == 1);
    REQUIRE(cfgNode5->getChild(0) == cfgNode1);

    REQUIRE(cfg->getMap()->size() == 5);
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
    REQUIRE(cfgNode1->getParents().size() == 1);

    shared_ptr<CFGNode> cfgNode2 = cfgNode1->getChild(0);
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
    REQUIRE(cfgNode5->getNumChildren() == 1);
    REQUIRE(cfgNode5->getChild(0) == cfgNode1);
    shared_ptr<CFGNode> cfgNode6 = cfgNode1->getChild(1);
    assignStatement = dynamic_pointer_cast<AssignStatement>(cfgNode6->getTNode());
    REQUIRE(assignStatement->getVarName() == "flag");
    constantExpression3 =
            dynamic_pointer_cast<ConstantExpression>(assignStatement->getRelFactor());
    REQUIRE(constantExpression3->getValue() == 5);
    REQUIRE(cfgNode6->getNumChildren() == 0);

    REQUIRE(cfg->getMap()->size() == 6);
}

TEST_CASE("Procedure with while statement and if statement - test 2") {
    string proc = "procedure main {\n"
                  "    flag = 5;"
                  "    while (flag >= 0) {\n"
                  "        if (flag >= 0) then {\n"
                  "            print flag;\n"
                  "        } else {\n"
                  "            read flag;\n"
                  "        }"
                  "    }\n"
                  "}\n";
    shared_ptr<Procedure> procedure = Parser::parseProcedure(proc);
    procedure->buildCFG(procedure->getProcedureName());
    shared_ptr<CFG> cfg = procedure->getCFG();
    REQUIRE(cfg->getName() == "main");

    shared_ptr<CFGNode> cfgNode1 = cfg->getStartNode();
    REQUIRE(cfgNode1->getNumChildren() == 1);
    shared_ptr<AssignStatement> assignStatement = dynamic_pointer_cast<AssignStatement>(cfgNode1->getTNode());
    REQUIRE(assignStatement->getVarName() == "flag");
    shared_ptr<ConstantExpression> constantExpression1 =
            dynamic_pointer_cast<ConstantExpression>(assignStatement->getRelFactor());
    REQUIRE(constantExpression1->getValue() == 5);

    shared_ptr<CFGNode> cfgNode2 = cfgNode1->getChild(0);
    shared_ptr<WhileStatement> whileStatement = dynamic_pointer_cast<WhileStatement>(cfgNode2->getTNode());
    shared_ptr<RelationalExpression> condExpr =
            dynamic_pointer_cast<RelationalExpression>(whileStatement->getConditionalExpression());
    shared_ptr<NameExpression> nameExpr = dynamic_pointer_cast<NameExpression>(condExpr->getRelFactor1());
    shared_ptr<ConstantExpression> constantExpression =
            dynamic_pointer_cast<ConstantExpression>(condExpr->getRelFactor2());
    REQUIRE(nameExpr->getVarName() == "flag");
    REQUIRE(constantExpression->getValue() == 0);
    REQUIRE(condExpr->getOperator() == RelationalOperator::GREATER_THAN_OR_EQUALS);
    REQUIRE(cfgNode2->getNumChildren() == 1);
    REQUIRE(cfgNode2->getParents().size() == 2);

    shared_ptr<CFGNode> cfgNode3 = cfgNode2->getChild(0);
    shared_ptr<IfStatement> ifStatement = dynamic_pointer_cast<IfStatement>(cfgNode3->getTNode());
    shared_ptr<RelationalExpression> condExpr2 =
            dynamic_pointer_cast<RelationalExpression>(ifStatement->getConditionalExpression());
    shared_ptr<NameExpression> nameExpr2 = dynamic_pointer_cast<NameExpression>(condExpr2->getRelFactor1());
    shared_ptr<ConstantExpression> constantExpression2 =
            dynamic_pointer_cast<ConstantExpression>(condExpr2->getRelFactor2());
    REQUIRE(nameExpr2->getVarName() == "flag");
    REQUIRE(constantExpression2->getValue() == 0);
    REQUIRE(condExpr2->getOperator() == RelationalOperator::GREATER_THAN_OR_EQUALS);
    REQUIRE(cfgNode3->getNumChildren() == 2);

    shared_ptr<CFGNode> cfgNode4 = cfgNode3->getChild(0);
    shared_ptr<PrintStatement> printStatement = dynamic_pointer_cast<PrintStatement>(cfgNode4->getTNode());
    REQUIRE(printStatement->getVariableName() == "flag");

    shared_ptr<CFGNode> dummyNode1 = cfgNode4->getChild(0);
    REQUIRE(dummyNode1->getNumChildren() == 1);

    shared_ptr<CFGNode> cfgNode5 = cfgNode3->getChild(1);
    shared_ptr<ReadStatement> readStatement = dynamic_pointer_cast<ReadStatement>(cfgNode5->getTNode());
    REQUIRE(readStatement->getVariableName() == "flag");

    dummyNode1 = cfgNode5->getChild(0);
    REQUIRE(dummyNode1->getNumChildren() == 1);
    REQUIRE(dummyNode1->getTNode() == nullptr);
    REQUIRE(dummyNode1->getChild(0) == cfgNode2);

    REQUIRE(cfg->getMap()->size() == 5);
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
    REQUIRE(cfgNode5->getNumChildren() == 1);
    REQUIRE(cfgNode5->getChild(0) == cfgNode1);
    shared_ptr<CFGNode> cfgNode6 = cfgNode1->getChild(1);
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
    cfgNode4 = cfgNode1->getChild(1);
    shared_ptr<PrintStatement> printStatement2 = dynamic_pointer_cast<PrintStatement>(cfgNode4->getTNode());
    REQUIRE(printStatement2->getVariableName() == "x");
    REQUIRE(cfgNode2->getNumChildren() == 1);

    cfgNode3 = cfgNode2->getChild(0);
    shared_ptr<ReadStatement> readStatement = dynamic_pointer_cast<ReadStatement>(cfgNode3->getTNode());
    REQUIRE(readStatement->getVariableName() == "flag");
    REQUIRE(cfgNode3->getNumChildren() == 1);
    cfgNode1 = cfgNode3->getChild(0);
    whileStatement = dynamic_pointer_cast<WhileStatement>(cfgNode1->getTNode());
    condExpr = dynamic_pointer_cast<RelationalExpression>(whileStatement->getConditionalExpression());
    nameExpr = dynamic_pointer_cast<NameExpression>(condExpr->getRelFactor1());
    constantExpression = dynamic_pointer_cast<ConstantExpression>(condExpr->getRelFactor2());
    REQUIRE(nameExpr->getVarName() == "flag");
    REQUIRE(constantExpression->getValue() == 0);
    REQUIRE(condExpr->getOperator() == RelationalOperator::GREATER_THAN);

    REQUIRE(cfg1->getMap()->size() == 6);
    REQUIRE(cfg2->getMap()->size() == 4);
}

TEST_CASE("Get all maps from source code") {
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

    shared_ptr<CFG> cfg1 = cfgLst[0];
    shared_ptr<CFGNode> cfgNode1 = cfg1->getStartNode();
    shared_ptr<CFGNode> cfgNode2 = cfgNode1->getChild(0);
    shared_ptr<CFGNode> cfgNode3 = cfgNode2->getChild(0);
    shared_ptr<CFGNode> dummyNode1 = cfgNode3->getChild(0);
    shared_ptr<CFGNode> cfgNode4 = cfgNode2->getChild(1);
    shared_ptr<CFGNode> cfgNode5 = dummyNode1->getChild(0);
    shared_ptr<CFGNode> cfgNode6 = cfgNode1->getChild(1);
    
    REQUIRE(cfgMap->at(1) == cfgNode1);
    REQUIRE(cfgMap->at(2) == cfgNode2);
    REQUIRE(cfgMap->at(3) == cfgNode3);
    REQUIRE(cfgMap->at(4) == cfgNode4);
    REQUIRE(cfgMap->at(5) == cfgNode5);
    REQUIRE(cfgMap->at(6) == cfgNode6);

    shared_ptr<CFG> cfg2 = cfgLst[1];
    cfgNode1 = cfg2->getStartNode();
    cfgNode2 = cfgNode1->getChild(0);
    cfgNode4 = cfgNode1->getChild(1);
    cfgNode3 = cfgNode2->getChild(0);

    REQUIRE(cfgMap->at(7) == cfgNode1);
    REQUIRE(cfgMap->at(8) == cfgNode2);
    REQUIRE(cfgMap->at(9) == cfgNode3);
    REQUIRE(cfgMap->at(10) == cfgNode4);
}

TEST_CASE("Get all maps from complex source code") {
    string srcCode = "procedure main {\n"
                     " print = x;\n"
                     " read x;\n"
                     " left = x;\n"
                     " call somethingHmm;\n"
                     " right = left + 2;\n"
                     " read temp;\n"
                     " radius = x + left;\n"
                     " temp = temp + 2;\n"
                     " radius1 = x + left;\n"
                     " call anotherOne;\n"
                     "}\n"
                     "\n"
                     "procedure somethingHmm {\n"
                     " while (iter <=  5) {\n"
                     "  x = right + 3 + 2 + left;\n"
                     "  temp = temp + 7;\n"
                     "  call anotherOne;\n"
                     " }\n"
                     " x = 5;\n"
                     " if (iter != 1) then {\n"
                     "  iter = iter + 1; \n"
                     "  length = radius * 10;\n"
                     "  breadth = radius + 10;\n"
                     " } else {\n"
                     "  while ((left + 1 > 2) || (radius + 1 > 2)) {\n"
                     "   call nestedBlocks;\n"
                     "   length = left + right;\n"
                     "  }\n"
                     " }\n"
                     "}\n"
                     "\n"
                     "procedure nestedBlocks {\n"
                     " if (iter != 1) then {\n"
                     "  iter = iter + 1; \n"
                     "  length = radius * 10;\n"
                     "  breadth = radius + 10;\n"
                     " } else {\n"
                     "  while ((left + 1 > 2) || (radius + 1 > 2)) {\n"
                     "   breadth = radius - 100;\n"
                     "   length = left + right;\n"
                     "   call anotherOne;\n"
                     "   if (1 > 2) then {\n"
                     "    procedure  = 5;\n"
                     "   } else {\n"
                     "    length = no;\n"
                     "    while ((x > 2) || !(y <= 5)) {\n"
                     "     call anotherOne;\n"
                     "    }\n"
                     "   }\n"
                     "  }\n"
                     " }\n"
                     "}\n"
                     "\n"
                     "procedure anotherOne {\n"
                     " print x;\n"
                     "}";
    Statement::resetLineNumCount();
    shared_ptr<SourceCode> sourceCode = Parser::parseSourceCode(srcCode, "");
    vector<shared_ptr<CFG> > cfgLst = sourceCode->getAllCFGs();

    shared_ptr<map<int, shared_ptr<CFGNode> >> cfgMap = sourceCode->getAllCFGMaps();

    shared_ptr<CFG> cfg1 = cfgLst[0];
    REQUIRE(cfg1->getMap()->size() == 10);

    shared_ptr<CFG> cfg2 = cfgLst[1];
    REQUIRE(cfg2->getMap()->size() == 12);

    shared_ptr<CFG> cfg3 = cfgLst[2];
    REQUIRE(cfg3->getMap()->size() == 13);

    shared_ptr<CFG> cfg4 = cfgLst[3];
    REQUIRE(cfg4->getMap()->size() == 1);
}

TEST_CASE("Procedure with while statement and double nested if statement") {
    string proc = "procedure a {\n"
                  "    while (difference==1) {\n"
                  "        if(x == 2) then {\n"
                  "            t = 3;\n"
                  "            if (y == 4) then{\n"
                  "                t = 5;\n"
                  "            } else {\n"
                  "                t = 6;\n"
                  "            }\n"
                  "        } else {\n"
                  "            t = 7;\n"
                  "        }\n"
                  "    }\n"
                  "}";
    shared_ptr<Procedure> procedure = Parser::parseProcedure(proc);
    procedure->buildCFG(procedure->getProcedureName());
    shared_ptr<CFG> cfg = procedure->getCFG();
    REQUIRE(cfg->getName() == "a");

    shared_ptr<CFGNode> cfgNode1 = cfg->getStartNode();
    REQUIRE(cfgNode1->getNumChildren() == 1);
    shared_ptr<WhileStatement> whileStatement = dynamic_pointer_cast<WhileStatement>(cfgNode1->getTNode());
    shared_ptr<RelationalExpression> condExpr = dynamic_pointer_cast<RelationalExpression>(whileStatement->getConditionalExpression());
    shared_ptr<NameExpression> nameExpr = dynamic_pointer_cast<NameExpression>(condExpr->getRelFactor1());
    shared_ptr<ConstantExpression> constantExpression = dynamic_pointer_cast<ConstantExpression>(condExpr->getRelFactor2());
    REQUIRE(nameExpr->getVarName() == "difference");
    REQUIRE(constantExpression->getValue() == 1);
    REQUIRE(condExpr->getOperator() == RelationalOperator::EQUALS);

    shared_ptr<CFGNode> cfgNode2 = cfgNode1->getChild(0);
    shared_ptr<IfStatement> ifStatement = dynamic_pointer_cast<IfStatement>(cfgNode2->getTNode());
    shared_ptr<RelationalExpression> condExpr2 = dynamic_pointer_cast<RelationalExpression>(ifStatement->getConditionalExpression());
    shared_ptr<NameExpression> nameExpr2 = dynamic_pointer_cast<NameExpression>(condExpr2->getRelFactor1());
    shared_ptr<ConstantExpression> constantExpression2 = dynamic_pointer_cast<ConstantExpression>(condExpr2->getRelFactor2());
    REQUIRE(nameExpr2->getVarName() == "x");
    REQUIRE(constantExpression2->getValue() == 2);
    REQUIRE(condExpr2->getOperator() == RelationalOperator::EQUALS);
    REQUIRE(cfgNode2->getNumChildren() == 2);

    shared_ptr<CFGNode> cfgNode3 = cfgNode2->getChild(0);
    shared_ptr<AssignStatement> assignStatement = dynamic_pointer_cast<AssignStatement>(cfgNode3->getTNode());
    REQUIRE(assignStatement->getVarName() == "t");
    shared_ptr<ConstantExpression> const3 = dynamic_pointer_cast<ConstantExpression>(assignStatement->getRelFactor());
    REQUIRE(const3->getValue() == 3);
    REQUIRE(cfgNode3->getNumChildren() == 1);

    shared_ptr<CFGNode> cfgNode4 = cfgNode3->getChild(0);
    shared_ptr<IfStatement> ifStatement2 = dynamic_pointer_cast<IfStatement>(cfgNode4->getTNode());
    shared_ptr<RelationalExpression> condExpr3 = dynamic_pointer_cast<RelationalExpression>(ifStatement2->getConditionalExpression());
    shared_ptr<NameExpression> nameExpr3 = dynamic_pointer_cast<NameExpression>(condExpr3->getRelFactor1());
    shared_ptr<ConstantExpression> constantExpression4 = dynamic_pointer_cast<ConstantExpression>(condExpr3->getRelFactor2());
    REQUIRE(nameExpr3->getVarName() == "y");
    REQUIRE(constantExpression4->getValue() == 4);
    REQUIRE(condExpr3->getOperator() == RelationalOperator::EQUALS);
    REQUIRE(cfgNode4->getNumChildren() == 2);

    shared_ptr<CFGNode> cfgNode5 = cfgNode4->getChild(0);
    shared_ptr<AssignStatement> assignStatement2 = dynamic_pointer_cast<AssignStatement>(cfgNode5->getTNode());
    REQUIRE(assignStatement2->getVarName() == "t");
    shared_ptr<ConstantExpression> const5 = dynamic_pointer_cast<ConstantExpression>(assignStatement2->getRelFactor());
    REQUIRE(const5->getValue() == 5);
    REQUIRE(cfgNode5->getNumChildren() == 1);

    shared_ptr<CFGNode> cfgNode6 = cfgNode4->getChild(1);
    shared_ptr<AssignStatement> assignStatement3 = dynamic_pointer_cast<AssignStatement>(cfgNode6->getTNode());
    REQUIRE(assignStatement3->getVarName() == "t");
    shared_ptr<ConstantExpression> const6 = dynamic_pointer_cast<ConstantExpression>(assignStatement3->getRelFactor());
    REQUIRE(const6->getValue() == 6);
    REQUIRE(cfgNode6->getNumChildren() == 1);

    shared_ptr<CFGNode> dummyNode1 = cfgNode6->getChild(0);
    REQUIRE(dummyNode1->getNumChildren() == 1);
    REQUIRE(dummyNode1->getTNode() == nullptr);
    REQUIRE(dummyNode1->getChild(0)->getTNode() == nullptr);

    shared_ptr<CFGNode> cfgNode7 = cfgNode2->getChild(1);
    shared_ptr<AssignStatement> assignStatement4 = dynamic_pointer_cast<AssignStatement>(cfgNode7->getTNode());
    REQUIRE(assignStatement4->getVarName() == "t");
    shared_ptr<ConstantExpression> const7 = dynamic_pointer_cast<ConstantExpression>(assignStatement4->getRelFactor());
    REQUIRE(const7->getValue() == 7);
    REQUIRE(cfgNode7->getNumChildren() == 1);

    shared_ptr<CFGNode> dummyNode2 = cfgNode7->getChild(0);
    REQUIRE(dummyNode2->getNumChildren() == 1);

    REQUIRE(cfg->getMap()->size() == 7);
}

TEST_CASE("Get all program level CFG information") {
    string srcCode = "procedure main {\n"
                     " print = x;\n"
                     " read x;\n"
                     " left = x;\n"
                     " call somethingHmm;\n"
                     " right = left + 2;\n"
                     " read temp;\n"
                     " radius = x + left;\n"
                     " temp = temp + 2;\n"
                     " radius1 = x + left;\n"
                     " call anotherOne;\n"
                     "}\n"
                     "\n"
                     "procedure somethingHmm {\n"
                     " while (iter <=  5) {\n"
                     "  x = right + 3 + 2 + left;\n"
                     "  temp = temp + 7;\n"
                     "  call anotherOne;\n"
                     " }\n"
                     " x = 5;\n"
                     " if (iter != 1) then {\n"
                     "  iter = iter + 1; \n"
                     "  length = radius * 10;\n"
                     "  breadth = radius + 10;\n"
                     " } else {\n"
                     "  while ((left + 1 > 2) || (radius + 1 > 2)) {\n"
                     "   call nestedBlocks;\n"
                     "   length = left + right;\n"
                     "  }\n"
                     " }\n"
                     "}\n"
                     "\n"
                     "procedure nestedBlocks {\n"
                     " if (iter != 1) then {\n"
                     "  iter = iter + 1; \n"
                     "  length = radius * 10;\n"
                     "  breadth = radius + 10;\n"
                     " } else {\n"
                     "  while ((left + 1 > 2) || (radius + 1 > 2)) {\n"
                     "   breadth = radius - 100;\n"
                     "   length = left + right;\n"
                     "   call anotherOne;\n"
                     "   if (1 > 2) then {\n"
                     "    procedure  = 5;\n"
                     "   } else {\n"
                     "    length = no;\n"
                     "    while ((x > 2) || !(y <= 5)) {\n"
                     "     call anotherOne;\n"
                     "    }\n"
                     "   }\n"
                     "  }\n"
                     " }\n"
                     "}\n"
                     "\n"
                     "procedure anotherOne {\n"
                     " print x;\n"
                     "}";
    Statement::resetLineNumCount();
    shared_ptr<SourceCode> sourceCode = Parser::parseSourceCode(srcCode, "");
    shared_ptr<AllCFGs> allCFGInfo = sourceCode->getAllCFGInfo();

    vector<shared_ptr<CFG> > cfgLst = allCFGInfo->getAllCFGs();
    shared_ptr<map<int, shared_ptr<CFGNode> >> combinedCfgMap = allCFGInfo->getCombinedMap();

    shared_ptr<CFG> cfg1 = cfgLst[0];
    REQUIRE(cfg1->getMap()->size() == 10);

    shared_ptr<CFG> cfg2 = cfgLst[1];
    REQUIRE(cfg2->getMap()->size() == 12);

    shared_ptr<CFG> cfg3 = cfgLst[2];
    REQUIRE(cfg3->getMap()->size() == 13);

    shared_ptr<CFG> cfg4 = cfgLst[3];
    REQUIRE(cfg4->getMap()->size() == 1);

    REQUIRE(combinedCfgMap->size() == 36);
}