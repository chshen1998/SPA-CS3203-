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
    shared_ptr<CFGNode> result = whileStatement->buildCFG(parents);

    REQUIRE(result->getParents().size() == 1);
    REQUIRE(result->getNumChildren() == 1);
    shared_ptr<CFGNode> cfgNode1 = result->getChild(0);
    shared_ptr<ReadStatement> stmt1 = dynamic_pointer_cast<ReadStatement>(cfgNode1->getTNode());
    REQUIRE(stmt1->getVariableName() == "x");
    REQUIRE(cfgNode1->getParents().size() == 1);
    REQUIRE(cfgNode1->getNumChildren() == 1);
    shared_ptr<CFGNode> cfgNode2 = cfgNode1->getChild(0);
    shared_ptr<PrintStatement> stmt2 = dynamic_pointer_cast<PrintStatement>(cfgNode2->getTNode());
    REQUIRE(stmt2->getVariableName() == "y");
    REQUIRE(cfgNode2->getNumChildren() == 1);
    shared_ptr<CFGNode> cfgNode3 = cfgNode2->getChild(0);
    REQUIRE(cfgNode3->getTNode() == whileStatement);
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
    shared_ptr<CFGNode> result = whileStatement->buildCFG(parents);

    REQUIRE(result->getParents().size() == 1);
    REQUIRE(result->getNumChildren() == 1);
    shared_ptr<CFGNode> cfgNode1 = result->getChild(0);
    shared_ptr<ReadStatement> stmt1 = dynamic_pointer_cast<ReadStatement>(cfgNode1->getTNode());
    REQUIRE(stmt1->getVariableName() == "x");
    REQUIRE(cfgNode1->getParents().size() == 1);
    REQUIRE(cfgNode1->getNumChildren() == 1);
    shared_ptr<CFGNode> cfgNode2 = cfgNode1->getChild(0);
    shared_ptr<WhileStatement> stmt2 = dynamic_pointer_cast<WhileStatement>(cfgNode2->getTNode());
    shared_ptr<RelationalExpression> relExpr =
            dynamic_pointer_cast<RelationalExpression>(stmt2->getConditionalExpression());
    shared_ptr<ConstantExpression> relFactor1 =
            dynamic_pointer_cast<ConstantExpression>(relExpr->getRelFactor1());
    shared_ptr<ConstantExpression> relFactor2 =
            dynamic_pointer_cast<ConstantExpression>(relExpr->getRelFactor2());
    REQUIRE(relFactor1->getValue() == 10);
    REQUIRE(relFactor2->getValue() == 11);
    REQUIRE(relExpr->getOperator() == RelationalOperator::EQUALS);
    REQUIRE(cfgNode2->getParents().size() == 2);
    REQUIRE(cfgNode2->getNumChildren() == 2);
    shared_ptr<CFGNode> cfgNode3 = cfgNode2->getChild(0);
    REQUIRE(cfgNode3->getTNode() == printStatement);
    REQUIRE(cfgNode3->getChild(0)->getTNode() == whileStatement2);
    REQUIRE(cfgNode2->getChild(1)->getTNode() == whileStatement);
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
}