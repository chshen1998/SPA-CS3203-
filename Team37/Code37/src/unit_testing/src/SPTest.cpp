#include "SP/Tokenizer.h"
#include "SP/Parser.h"
#include "SP/InvalidSyntaxException.h"

#include <catch.hpp>
#include <string>
#include <vector>

using namespace std;

void require(bool b);

TEST_CASE("Extract Procedures") {
    vector<string> procedures;
    string p = "procedure main {\n"
                      "    flag = 0;\n"
                      "    print flag;\n"
                      "    read flag;\n"
                      "\n"
                      "    x = 1;\n"
                      "    y = 2;\n"
                      "    print x;\n"
                      "}";
    procedures.push_back(p);

    vector<string> result;
    result = Parser::extractProcedures(p, result);

    REQUIRE(result == procedures);
}

TEST_CASE("Extract procedure names") {
    string procedure = "procedure main {\n"
                       "    flag = 0;\n"
                       "    print flag;\n"
                       "    read flag;\n"
                       "\n"
                       "    x = 1;\n"
                       "    y = 2;\n"
                       "    print x;\n"
                       "}";

    string result = Parser::extractProcName(procedure);

    string expectedName = "main";
    REQUIRE(expectedName == result);
}

TEST_CASE("Remove procedure wrapper - Good input") {
    string procedure = "procedure main {\n"
                       "    flag = 0;\n"
                       "    print flag;\n"
                       "    read flag;\n"
                       "\n"
                       "    x = 1;\n"
                       "    y = 2;\n"
                       "    print x;\n"
                       "}";

    string result = Parser::removeProcedureWrapper(procedure);
    string expected = "flag = 0;\n"
                      "    print flag;\n"
                      "    read flag;\n"
                      "\n"
                      "    x = 1;\n"
                      "    y = 2;\n"
                      "    print x;";

    REQUIRE(result == expected);

    // With while and if-else blocks
    procedure = "procedure main {\n"
               "    while () {\n"
               "        print flag;\n"
               "        read flag;\n"
               "\n"
               "        x = 1;\n"
               "        y = 2;\n"
               "    }\n"
               "    print x;\n"
               "    if () then {\n"
               "        print flag;\n"
               "    } else {\n"
               "        read flag;\n"
               "    }"
               "}\n";

    result = Parser::removeProcedureWrapper(procedure);
    expected = "while () {\n"
               "        print flag;\n"
               "        read flag;\n"
               "\n"
               "        x = 1;\n"
               "        y = 2;\n"
               "    }\n"
               "    print x;\n"
               "    if () then {\n"
               "        print flag;\n"
               "    } else {\n"
               "        read flag;\n"
               "    }";

    REQUIRE(result == expected);
}


TEST_CASE("Extract Statements - No while and if-else") {
    string rawStatementList = "flag = 0;\n"
                       "    print flag;\n"
                       "    read flag;\n"
                       "\n"
                       "    x = 1;\n"
                       "    y = 2;\n"
                       "print x;";

    vector<string> statementList;

    vector<string> result = Parser::extractStatements(rawStatementList, statementList);
    vector<string> expected;
    string stmt1 = "flag = 0";
    string stmt2 = "print flag";
    string stmt3 = "read flag";
    string stmt4 = "x = 1";
    string stmt5 = "y = 2";
    string stmt6 = "print x";

    expected.push_back(stmt1);
    expected.push_back(stmt2);
    expected.push_back(stmt3);
    expected.push_back(stmt4);
    expected.push_back(stmt5);
    expected.push_back(stmt6);

    REQUIRE(result == expected);
}

TEST_CASE("Extract Statements - While and if-else included") {
    string rawStatementList = "while () {\n"
                       "    print flag;\n"
                       "    read flag;\n"
                       "\n"
                       "    x = 1;\n"
                       "    y = 2;\n"
                       "    }\n"
                       "    print x;\n"
                       "    if () then {\n"
                       "        print flag;\n"
                       "    } else {\n"
                       "        read flag;\n"
                       "    }";

    vector<string> statementList;

    vector<string> result = Parser::extractStatements(rawStatementList, statementList);
    vector<string> expected;
    string stmt1 = "while () {\n"
                   "    print flag;\n"
                   "    read flag;\n"
                   "\n"
                   "    x = 1;\n"
                   "    y = 2;\n"
                   "    }";
    string stmt2 = "print x";
    string stmt3 = "if () then {\n"
                    "        print flag;\n"
                    "    } else {\n"
                    "        read flag;\n"
                    "    }";

    expected.push_back(stmt1);
    expected.push_back(stmt2);
    expected.push_back(stmt3);

    REQUIRE(result == expected);
}

TEST_CASE("Extract Statements - Nested While and if-else") {
    string rawStatementList = "while () {\n"
                              "\t\tcall x;\n"
                              "\t\tread x;\n"
                              "\t\twhile () {\n"
                              "\t\t\t\tread x;\n"
                              "\t\t}\n"
                              "\t\tif () then {\n"
                              "\t\t\t\tread x;\n"
                              "\t\t} else {\n"
                              "\t\t\t\tread x;\n"
                              "\t\t}\n"
                              "}\n"
                              "\n"
                              "if () then {\n"
                              "\t\twhile () {\n"
                              "\t\t\t\tread x;\n"
                              "\t\t}\n"
                              "} else {\n"
                              "\t\tif () then {\n"
                              "\t\t\t\tread x;\n"
                              "\t\t} else {\n"
                              "\t\t\t\tread x;\n"
                              "\t\t}\n"
                              "}";
    vector<string> statementList;
    vector<string> result = Parser::extractStatements(rawStatementList, statementList);

    vector<string> expected;
    string stmt1 = "while () {\n"
                   "\t\tcall x;\n"
                   "\t\tread x;\n"
                   "\t\twhile () {\n"
                   "\t\t\t\tread x;\n"
                   "\t\t}\n"
                   "\t\tif () then {\n"
                   "\t\t\t\tread x;\n"
                   "\t\t} else {\n"
                   "\t\t\t\tread x;\n"
                   "\t\t}\n"
                   "}";
    string stmt2 = "if () then {\n"
                   "\t\twhile () {\n"
                   "\t\t\t\tread x;\n"
                   "\t\t}\n"
                   "} else {\n"
                   "\t\tif () then {\n"
                   "\t\t\t\tread x;\n"
                   "\t\t} else {\n"
                   "\t\t\t\tread x;\n"
                   "\t\t}\n"
                   "}";

    expected.push_back(stmt1);
    expected.push_back(stmt2);

    REQUIRE(result == expected);
}

TEST_CASE("Tokenize read") {
    string line = "read v";
    shared_ptr<ReadStatement> result = Tokenizer::tokenizeRead(line);

    REQUIRE(result->getVariableName() == "v");
}

TEST_CASE("Tokenize print") {
    string line = "print v";
    shared_ptr<PrintStatement> result = Tokenizer::tokenizePrint(line);

    REQUIRE(result->getVariableName() == "v");
}

TEST_CASE("Tokenize RelFactor - Simple Addition / Subtraction") {
    string rawAssignStatement = "a - 1 + hmm - 9312";
    shared_ptr<RelationalFactor> relFactor = Tokenizer::tokenizeRelFactor(rawAssignStatement);

    shared_ptr<NameExpression> nameExpression;
    shared_ptr<ConstantExpression> constantExpression;
    shared_ptr<OperatedExpression> operatedExpression;

    operatedExpression = dynamic_pointer_cast<OperatedExpression>(relFactor);
    constantExpression = dynamic_pointer_cast<ConstantExpression>(operatedExpression->getExpression2());
    REQUIRE(operatedExpression->getOperator() == Operator::SUBTRACT);
    REQUIRE(constantExpression->getValue() == 9312);

    operatedExpression = dynamic_pointer_cast<OperatedExpression>(operatedExpression->getExpression1());
    nameExpression = dynamic_pointer_cast<NameExpression>(operatedExpression->getExpression2());
    REQUIRE(operatedExpression->getOperator() == Operator::ADD);
    REQUIRE(nameExpression->getVarName() == "hmm");

    operatedExpression = dynamic_pointer_cast<OperatedExpression>(operatedExpression->getExpression1());
    nameExpression = dynamic_pointer_cast<NameExpression>(operatedExpression->getExpression1());
    REQUIRE(operatedExpression->getOperator() == Operator::SUBTRACT);
    REQUIRE(nameExpression->getVarName() == "a");

    constantExpression = dynamic_pointer_cast<ConstantExpression>(operatedExpression->getExpression2());

    REQUIRE(constantExpression->getValue() == 1);
}

TEST_CASE("Tokenize RelFactor - Simple Multiplication / Division / Modulo") {
    string rawAssignStatement = "a % 1 / hmm * 9312";
    shared_ptr<RelationalFactor> relFactor = Tokenizer::tokenizeRelFactor(rawAssignStatement);

    shared_ptr<NameExpression> nameExpression;
    shared_ptr<ConstantExpression> constantExpression;
    shared_ptr<OperatedExpression> operatedExpression;

    operatedExpression = dynamic_pointer_cast<OperatedExpression>(relFactor);
    constantExpression = dynamic_pointer_cast<ConstantExpression>(operatedExpression->getExpression2());
    REQUIRE(operatedExpression->getOperator() == Operator::MULTIPLY);
    REQUIRE(constantExpression->getValue() == 9312);

    operatedExpression = dynamic_pointer_cast<OperatedExpression>(operatedExpression->getExpression1());
    nameExpression = dynamic_pointer_cast<NameExpression>(operatedExpression->getExpression2());
    REQUIRE(operatedExpression->getOperator() == Operator::DIVIDE);
    REQUIRE(nameExpression->getVarName() == "hmm");

    operatedExpression = dynamic_pointer_cast<OperatedExpression>(operatedExpression->getExpression1());
    nameExpression = dynamic_pointer_cast<NameExpression>(operatedExpression->getExpression1());
    REQUIRE(operatedExpression->getOperator() == Operator::MODULO);
    REQUIRE(nameExpression->getVarName() == "a");

    constantExpression = dynamic_pointer_cast<ConstantExpression>(operatedExpression->getExpression2());

    REQUIRE(constantExpression->getValue() == 1);
}

TEST_CASE("Tokenize RelFactor - Simple, ALL Operators") {
    string rawAssignStatement = "a % b - c / d + e % f * g + h - i";
    shared_ptr<RelationalFactor> relFactor = Tokenizer::tokenizeRelFactor(rawAssignStatement);

    shared_ptr<NameExpression> nameExpression;
    shared_ptr<OperatedExpression> operatedExpression;
    shared_ptr<OperatedExpression> savePoint;

    operatedExpression = dynamic_pointer_cast<OperatedExpression>(relFactor);
    REQUIRE(operatedExpression->getOperator() == Operator::SUBTRACT);
    nameExpression = dynamic_pointer_cast<NameExpression>(operatedExpression->getExpression2());
    REQUIRE(nameExpression->getVarName() == "i");

    operatedExpression = dynamic_pointer_cast<OperatedExpression>(operatedExpression->getExpression1());
    REQUIRE(operatedExpression->getOperator() == Operator::ADD);
    nameExpression = dynamic_pointer_cast<NameExpression>(operatedExpression->getExpression2());
    REQUIRE(nameExpression->getVarName() == "h");

    operatedExpression = dynamic_pointer_cast<OperatedExpression>(operatedExpression->getExpression1());
    REQUIRE(operatedExpression->getOperator() == Operator::ADD);

    savePoint = operatedExpression;

    // e % f * g
    operatedExpression = dynamic_pointer_cast<OperatedExpression>(savePoint->getExpression2());
    REQUIRE(operatedExpression->getOperator() == Operator::MULTIPLY);
    nameExpression = dynamic_pointer_cast<NameExpression>(operatedExpression->getExpression2());
    REQUIRE(nameExpression->getVarName() == "g");

    operatedExpression = dynamic_pointer_cast<OperatedExpression>(operatedExpression->getExpression1());
    REQUIRE(operatedExpression->getOperator() == Operator::MODULO);
    nameExpression = dynamic_pointer_cast<NameExpression>(operatedExpression->getExpression2());
    REQUIRE(nameExpression->getVarName() == "f");
    nameExpression = dynamic_pointer_cast<NameExpression>(operatedExpression->getExpression1());
    REQUIRE(nameExpression->getVarName() == "e");

    // a % b - c / d
    operatedExpression = dynamic_pointer_cast<OperatedExpression>(savePoint->getExpression1());
    savePoint = operatedExpression;
    REQUIRE(operatedExpression->getOperator() == Operator::SUBTRACT);

    operatedExpression = dynamic_pointer_cast<OperatedExpression>(operatedExpression->getExpression2());
    REQUIRE(operatedExpression->getOperator() == Operator::DIVIDE);
    nameExpression = dynamic_pointer_cast<NameExpression>(operatedExpression->getExpression2());
    REQUIRE(nameExpression->getVarName() == "d");
    nameExpression = dynamic_pointer_cast<NameExpression>(operatedExpression->getExpression1());
    REQUIRE(nameExpression->getVarName() == "c");

    operatedExpression = dynamic_pointer_cast<OperatedExpression>(savePoint->getExpression1());
    REQUIRE(operatedExpression->getOperator() == Operator::MODULO);
    nameExpression = dynamic_pointer_cast<NameExpression>(operatedExpression->getExpression2());
    REQUIRE(nameExpression->getVarName() == "b");
    nameExpression = dynamic_pointer_cast<NameExpression>(operatedExpression->getExpression1());
    REQUIRE(nameExpression->getVarName() == "a");
}

TEST_CASE("Tokenize RelFactor - Annoying Brackets") {
    string rawAssignStatement = "(((a + b)))";
    shared_ptr<RelationalFactor> relFactor = Tokenizer::tokenizeRelFactor(rawAssignStatement);

    shared_ptr<NameExpression> nameExpression;
    shared_ptr<OperatedExpression> operatedExpression;

    operatedExpression = dynamic_pointer_cast<OperatedExpression>(relFactor);
    REQUIRE(operatedExpression->getOperator() == Operator::ADD);
    nameExpression = dynamic_pointer_cast<NameExpression>(operatedExpression->getExpression1());
    REQUIRE(nameExpression->getVarName() == "a");
    nameExpression = dynamic_pointer_cast<NameExpression>(operatedExpression->getExpression2());
    REQUIRE(nameExpression->getVarName() == "b");
}

TEST_CASE("Tokenize RelFactor - Brackets reversing the order") {
    string rawAssignStatement = "(a + (b + (c + d)))";
    shared_ptr<RelationalFactor> relFactor = Tokenizer::tokenizeRelFactor(rawAssignStatement);

    shared_ptr<NameExpression> nameExpression;
    shared_ptr<OperatedExpression> operatedExpression;

    operatedExpression = dynamic_pointer_cast<OperatedExpression>(relFactor);
    REQUIRE(operatedExpression->getOperator() == Operator::ADD);
    nameExpression = dynamic_pointer_cast<NameExpression>(operatedExpression->getExpression1());
    REQUIRE(nameExpression->getVarName() == "a");

    operatedExpression = dynamic_pointer_cast<OperatedExpression>(operatedExpression->getExpression2());
    REQUIRE(operatedExpression->getOperator() == Operator::ADD);
    nameExpression = dynamic_pointer_cast<NameExpression>(operatedExpression->getExpression1());
    REQUIRE(nameExpression->getVarName() == "b");

    operatedExpression = dynamic_pointer_cast<OperatedExpression>(operatedExpression->getExpression2());
    REQUIRE(operatedExpression->getOperator() == Operator::ADD);
    nameExpression = dynamic_pointer_cast<NameExpression>(operatedExpression->getExpression1());
    REQUIRE(nameExpression->getVarName() == "c");
    nameExpression = dynamic_pointer_cast<NameExpression>(operatedExpression->getExpression2());
    REQUIRE(nameExpression->getVarName() == "d");
}

TEST_CASE("Tokenize RelFactor - Brackets changing priority") {
    string rawAssignStatement = "((a % b) - c)";
    shared_ptr<RelationalFactor> relFactor = Tokenizer::tokenizeRelFactor(rawAssignStatement);

    shared_ptr<NameExpression> nameExpression;
    shared_ptr<OperatedExpression> operatedExpression;
    operatedExpression = dynamic_pointer_cast<OperatedExpression>(relFactor);
    REQUIRE(operatedExpression->getOperator() == Operator::SUBTRACT);
    nameExpression = dynamic_pointer_cast<NameExpression>(operatedExpression->getExpression2());
    REQUIRE(nameExpression->getVarName() == "c");

    operatedExpression = dynamic_pointer_cast<OperatedExpression>(operatedExpression->getExpression1());
    REQUIRE(operatedExpression->getOperator() == Operator::MODULO);
    nameExpression = dynamic_pointer_cast<NameExpression>(operatedExpression->getExpression1());
    REQUIRE(nameExpression->getVarName() == "a");
    nameExpression = dynamic_pointer_cast<NameExpression>(operatedExpression->getExpression2());
    REQUIRE(nameExpression->getVarName() == "b");


    rawAssignStatement = "(a % (b - c))";
    relFactor = Tokenizer::tokenizeRelFactor(rawAssignStatement);

    operatedExpression = dynamic_pointer_cast<OperatedExpression>(relFactor);
    REQUIRE(operatedExpression->getOperator() == Operator::MODULO);
    nameExpression = dynamic_pointer_cast<NameExpression>(operatedExpression->getExpression1());
    REQUIRE(nameExpression->getVarName() == "a");

    operatedExpression = dynamic_pointer_cast<OperatedExpression>(operatedExpression->getExpression2());
    REQUIRE(operatedExpression->getOperator() == Operator::SUBTRACT);
    nameExpression = dynamic_pointer_cast<NameExpression>(operatedExpression->getExpression1());
    REQUIRE(nameExpression->getVarName() == "b");
    nameExpression = dynamic_pointer_cast<NameExpression>(operatedExpression->getExpression2());
    REQUIRE(nameExpression->getVarName() == "c");
}


TEST_CASE("Extract Conditional Expression") {
    string ifStatementStr = "if ((x != 9) && (x <= y))";
    string expected = "(x != 9) && (x <= y)";
    string result = Parser::extractConditionalExpr(ifStatementStr);
    REQUIRE(result == expected);
}

TEST_CASE("Parse relational expression") {
    string expr1 = "x >= 5";
    shared_ptr<RelationalExpression> relExpr1 = Parser::parseRelExpr(expr1);
    shared_ptr<NameExpression> nameExpr = dynamic_pointer_cast<NameExpression>(relExpr1->getRelFactor1());
    shared_ptr<ConstantExpression> constExpr = dynamic_pointer_cast<ConstantExpression>(relExpr1->getRelFactor2());
    REQUIRE(relExpr1->getOperator() == RelationalOperator::GREATER_THAN_OR_EQUALS);
    REQUIRE(nameExpr->getVarName() == "x");
    REQUIRE(constExpr->getValue() == 5);

    string expr2 = "x > y";
    shared_ptr<RelationalExpression> relExpr2 = Parser::parseRelExpr(expr2);
    REQUIRE(relExpr2->getOperator() == RelationalOperator::GREATER_THAN);
    nameExpr = dynamic_pointer_cast<NameExpression>(relExpr2->getRelFactor1());
    REQUIRE(nameExpr->getVarName() == "x");
    nameExpr = dynamic_pointer_cast<NameExpression>(relExpr2->getRelFactor2());
    REQUIRE(nameExpr->getVarName() == "y");

    string expr3 = "x == 5";
    shared_ptr<RelationalExpression> relExpr3 = Parser::parseRelExpr(expr3);
    REQUIRE(relExpr3->getOperator() == RelationalOperator::EQUALS);
    nameExpr = dynamic_pointer_cast<NameExpression>(relExpr3->getRelFactor1());
    constExpr = dynamic_pointer_cast<ConstantExpression>(relExpr3->getRelFactor2());
    REQUIRE(nameExpr->getVarName() == "x");
    REQUIRE(constExpr->getValue() == 5);

    string expr4 = "x != (y + 5)";
    shared_ptr<RelationalExpression> relExpr4 = Parser::parseRelExpr(expr4);
    REQUIRE(relExpr4->getOperator() == RelationalOperator::NOT_EQUALS);
    nameExpr = dynamic_pointer_cast<NameExpression>(relExpr4->getRelFactor1());
    shared_ptr<OperatedExpression> opExpr = dynamic_pointer_cast<OperatedExpression>(relExpr4->getRelFactor2());
    REQUIRE(nameExpr->getVarName() == "x");
    REQUIRE(opExpr->getOperator() == Operator::ADD);
    nameExpr = dynamic_pointer_cast<NameExpression>(opExpr->getExpression1());
    REQUIRE(nameExpr->getVarName() == "y");
    constExpr = dynamic_pointer_cast<ConstantExpression>(opExpr->getExpression2());
    REQUIRE(constExpr->getValue() == 5);
}

TEST_CASE("Parse Conditional Expression - Relational Condition") {
    string str = "x <= 5";
    shared_ptr<RelationalExpression> result = dynamic_pointer_cast<RelationalExpression>(Parser::parseCondExpr(str));
    REQUIRE(result->getOperator() == RelationalOperator::LESS_THAN_OR_EQUALS);
    shared_ptr<NameExpression> nameExpr = dynamic_pointer_cast<NameExpression>(result->getRelFactor1());
    REQUIRE(nameExpr->getVarName() == "x");
    shared_ptr<ConstantExpression> constExpr = dynamic_pointer_cast<ConstantExpression>(result->getRelFactor2());
    REQUIRE(constExpr->getValue() == 5);
}

TEST_CASE("Parse Conditional Expression - Relational Condition with Extra bracket") {
    string str = "(x <= 5)";
    shared_ptr<RelationalExpression> result = dynamic_pointer_cast<RelationalExpression>(Parser::parseCondExpr(str));
    REQUIRE(result->getOperator() == RelationalOperator::LESS_THAN_OR_EQUALS);
    shared_ptr<NameExpression> nameExpr = dynamic_pointer_cast<NameExpression>(result->getRelFactor1());
    REQUIRE(nameExpr->getVarName() == "x");
    shared_ptr<ConstantExpression> constExpr = dynamic_pointer_cast<ConstantExpression>(result->getRelFactor2());
    REQUIRE(constExpr->getValue() == 5);
}

TEST_CASE("Parse Conditional Expression - Not Condition with Relational Expression") {
    string str = "!(x <= 5)";
    shared_ptr<NotCondition> result = dynamic_pointer_cast<NotCondition>(Parser::parseCondExpr(str));
    shared_ptr<RelationalExpression> condExpr = dynamic_pointer_cast<RelationalExpression>(result->getConditionalExpression());
    REQUIRE(condExpr->getOperator() == RelationalOperator::LESS_THAN_OR_EQUALS);
    shared_ptr<NameExpression> nameExpr = dynamic_pointer_cast<NameExpression>(condExpr->getRelFactor1());
    REQUIRE(nameExpr->getVarName() == "x");
    shared_ptr<ConstantExpression> constExpr = dynamic_pointer_cast<ConstantExpression>(condExpr->getRelFactor2());
    REQUIRE(constExpr->getValue() == 5);
}

TEST_CASE("Parse Conditional Expression - Not Condition with nested Or conditional expression") {
    string str = "!((x <= 5) || (x == 10))";
    shared_ptr<NotCondition> result = dynamic_pointer_cast<NotCondition>(Parser::parseCondExpr(str));
    shared_ptr<OrCondition> orExpr = dynamic_pointer_cast<OrCondition>(result->getConditionalExpression());
    shared_ptr<RelationalExpression> relExpr1 = dynamic_pointer_cast<RelationalExpression>(orExpr->getConditionalExpression1());
    shared_ptr<RelationalExpression> relExpr2 = dynamic_pointer_cast<RelationalExpression>(orExpr->getConditionalExpression2());

    REQUIRE(relExpr1->getOperator() == RelationalOperator::LESS_THAN_OR_EQUALS);
    shared_ptr<NameExpression> nameExpr = dynamic_pointer_cast<NameExpression>(relExpr1->getRelFactor1());
    REQUIRE(nameExpr->getVarName() == "x");
    shared_ptr<ConstantExpression> constExpr = dynamic_pointer_cast<ConstantExpression>(relExpr1->getRelFactor2());
    REQUIRE(constExpr->getValue() == 5);

    REQUIRE(relExpr2->getOperator() == RelationalOperator::EQUALS);
    nameExpr = dynamic_pointer_cast<NameExpression>(relExpr2->getRelFactor1());
    REQUIRE(nameExpr->getVarName() == "x");
    constExpr = dynamic_pointer_cast<ConstantExpression>(relExpr2->getRelFactor2());
    REQUIRE(constExpr->getValue() == 10);
}

TEST_CASE("Parse conditional expression - Nested conditional expressions") {
    string str = "((x < y) || (x == y)) && !(x > 3)";
    shared_ptr<AndCondition> result = dynamic_pointer_cast<AndCondition>(Parser::parseCondExpr(str));
    shared_ptr<OrCondition> orCond = dynamic_pointer_cast<OrCondition>(result->getConditionalExpression1());

    shared_ptr<RelationalExpression> relExpr1 = dynamic_pointer_cast<RelationalExpression>(orCond->getConditionalExpression1());
    REQUIRE(relExpr1->getOperator() == RelationalOperator::LESS_THAN);
    shared_ptr<NameExpression> nameExpr = dynamic_pointer_cast<NameExpression>(relExpr1->getRelFactor1());
    REQUIRE(nameExpr->getVarName() == "x");
    nameExpr = dynamic_pointer_cast<NameExpression>(relExpr1->getRelFactor2());
    REQUIRE(nameExpr->getVarName() == "y");

    shared_ptr<RelationalExpression> relExpr2 = dynamic_pointer_cast<RelationalExpression>(orCond->getConditionalExpression2());
    REQUIRE(relExpr2->getOperator() == RelationalOperator::EQUALS);
    shared_ptr<NameExpression> nameExpr2 = dynamic_pointer_cast<NameExpression>(relExpr2->getRelFactor1());
    REQUIRE(nameExpr2->getVarName() == "x");
    nameExpr2 = dynamic_pointer_cast<NameExpression>(relExpr2->getRelFactor2());
    REQUIRE(nameExpr2->getVarName() == "y"); //Fail

    shared_ptr<NotCondition> notCond = dynamic_pointer_cast<NotCondition>(result->getConditionalExpression2());
    shared_ptr<RelationalExpression> relExpr3 = dynamic_pointer_cast<RelationalExpression>(notCond->getConditionalExpression());
    REQUIRE(relExpr3->getOperator() == RelationalOperator::GREATER_THAN);
    nameExpr = dynamic_pointer_cast<NameExpression>(relExpr3->getRelFactor1());
    REQUIRE(nameExpr->getVarName() == "x");
    shared_ptr<ConstantExpression> constExpr = dynamic_pointer_cast<ConstantExpression>(relExpr3->getRelFactor2());
    REQUIRE(constExpr->getValue() == 3);
}

TEST_CASE("Parse conditional with nested Not") {
    string str = "(x > y) || !(x == 5)";
    shared_ptr<OrCondition> orCond = dynamic_pointer_cast<OrCondition>(Parser::parseCondExpr(str));
    shared_ptr<RelationalExpression> rel1 = dynamic_pointer_cast<RelationalExpression>(orCond->getConditionalExpression1());
    shared_ptr<NotCondition> notExpr = dynamic_pointer_cast<NotCondition>(orCond->getConditionalExpression2());

    REQUIRE(rel1->getOperator() == RelationalOperator::GREATER_THAN);
    shared_ptr<NameExpression> nameExpr = dynamic_pointer_cast<NameExpression>(rel1->getRelFactor1());
    REQUIRE(nameExpr->getVarName() == "x");
    nameExpr = dynamic_pointer_cast<NameExpression>(rel1->getRelFactor2());
    REQUIRE(nameExpr->getVarName() == "y");

    shared_ptr<RelationalExpression> relExpr = dynamic_pointer_cast<RelationalExpression>(notExpr->getConditionalExpression());
    REQUIRE(relExpr->getOperator() == RelationalOperator::EQUALS);
    nameExpr = dynamic_pointer_cast<NameExpression>(relExpr->getRelFactor1());
    REQUIRE(nameExpr->getVarName() == "x");
    shared_ptr<ConstantExpression> constExpr = dynamic_pointer_cast<ConstantExpression>(relExpr->getRelFactor2());
    REQUIRE(constExpr->getValue() == 5);
}

TEST_CASE("Parse complex nested conditional") {
    string str = "!((x > 5) && (x < (y + 3))) || ((x==3) && (y!=1))";
    shared_ptr<OrCondition> orCondition = dynamic_pointer_cast<OrCondition>(Parser::parseCondExpr(str));
    shared_ptr<NotCondition> notCondition = dynamic_pointer_cast<NotCondition>(orCondition->getConditionalExpression1());
    shared_ptr<AndCondition> andCondition = dynamic_pointer_cast<AndCondition>(notCondition->getConditionalExpression());
    shared_ptr<RelationalExpression> relExpr1 = dynamic_pointer_cast<RelationalExpression>(andCondition->getConditionalExpression1());
    REQUIRE(relExpr1->getOperator() == RelationalOperator::GREATER_THAN);
    shared_ptr<NameExpression> nameExpr = dynamic_pointer_cast<NameExpression>(relExpr1->getRelFactor1());
    REQUIRE(nameExpr->getVarName() == "x");
    shared_ptr<ConstantExpression> constExpr = dynamic_pointer_cast<ConstantExpression>(relExpr1->getRelFactor2());
    REQUIRE(constExpr->getValue() == 5);
    shared_ptr<RelationalExpression> relExpr2 = dynamic_pointer_cast<RelationalExpression>(andCondition->getConditionalExpression2());
    REQUIRE(relExpr2->getOperator() == RelationalOperator::LESS_THAN);
    shared_ptr<NameExpression> nameExpr2 = dynamic_pointer_cast<NameExpression>(relExpr2->getRelFactor1());
    REQUIRE(nameExpr2->getVarName() == "x");
    shared_ptr<OperatedExpression> operatedExpression = dynamic_pointer_cast<OperatedExpression>(relExpr2->getRelFactor2());
    REQUIRE(operatedExpression->getOperator() == Operator::ADD);
    shared_ptr<NameExpression> nameExpr3 = dynamic_pointer_cast<NameExpression>(operatedExpression->getExpression1());
    REQUIRE(nameExpr3->getVarName() == "y");
    shared_ptr<ConstantExpression> constExpr2 = dynamic_pointer_cast<ConstantExpression>(operatedExpression->getExpression2());
    andCondition = dynamic_pointer_cast<AndCondition>(orCondition->getConditionalExpression2());
    relExpr1 = dynamic_pointer_cast<RelationalExpression>(andCondition->getConditionalExpression1());
    REQUIRE(relExpr1->getOperator() == RelationalOperator::EQUALS);
    nameExpr = dynamic_pointer_cast<NameExpression>(relExpr1->getRelFactor1());
    REQUIRE(nameExpr->getVarName() == "x");
    constExpr = dynamic_pointer_cast<ConstantExpression>(relExpr1->getRelFactor2());
    REQUIRE(constExpr->getValue() == 3);
    relExpr2 = dynamic_pointer_cast<RelationalExpression>(andCondition->getConditionalExpression2());
    REQUIRE(relExpr2->getOperator() == RelationalOperator::NOT_EQUALS);
    nameExpr = dynamic_pointer_cast<NameExpression>(relExpr2->getRelFactor1());
    REQUIRE(nameExpr->getVarName() == "y");
    constExpr = dynamic_pointer_cast<ConstantExpression>(relExpr2->getRelFactor2());
    REQUIRE(constExpr->getValue() == 1);
}

TEST_CASE("Nested not relational expression") {
    string str = "((x==3) && (y!=1))";
    shared_ptr<AndCondition> andCondition = dynamic_pointer_cast<AndCondition>(Parser::parseCondExpr(str));
    shared_ptr<RelationalExpression> relExpr1 = dynamic_pointer_cast<RelationalExpression>(andCondition->getConditionalExpression1());
    REQUIRE(relExpr1->getOperator() == RelationalOperator::EQUALS);
    shared_ptr<NameExpression> nameExpr = dynamic_pointer_cast<NameExpression>(relExpr1->getRelFactor1());
    REQUIRE(nameExpr->getVarName() == "x");
    shared_ptr<ConstantExpression> constExpr = dynamic_pointer_cast<ConstantExpression>(relExpr1->getRelFactor2());
    REQUIRE(constExpr->getValue() == 3);
    shared_ptr<RelationalExpression> relExpr2 = dynamic_pointer_cast<RelationalExpression>(andCondition->getConditionalExpression2());
    REQUIRE(relExpr2->getOperator() == RelationalOperator::NOT_EQUALS);
    nameExpr = dynamic_pointer_cast<NameExpression>(relExpr2->getRelFactor1());
    REQUIRE(nameExpr->getVarName() == "y");
    constExpr = dynamic_pointer_cast<ConstantExpression>(relExpr2->getRelFactor2());
    REQUIRE(constExpr->getValue() == 1);
}

TEST_CASE("Not relational expr") {
    string str = "!(y!=1)";
    shared_ptr<NotCondition> notCondition = dynamic_pointer_cast<NotCondition>(Parser::parseCondExpr(str));
    shared_ptr<RelationalExpression> relExpr1 = dynamic_pointer_cast<RelationalExpression>(notCondition->getConditionalExpression());
    REQUIRE(relExpr1->getOperator() == RelationalOperator::NOT_EQUALS);
    shared_ptr<NameExpression> nameExpr = dynamic_pointer_cast<NameExpression>(relExpr1->getRelFactor1());
    REQUIRE(nameExpr->getVarName() == "y");
    shared_ptr<ConstantExpression> constExpr = dynamic_pointer_cast<ConstantExpression>(relExpr1->getRelFactor2());
    REQUIRE(constExpr->getValue() == 1);
}

TEST_CASE("Extra brackets") {
    string str = "((!((x <= 5))))";
    shared_ptr<NotCondition> result = dynamic_pointer_cast<NotCondition>(Parser::parseCondExpr(str));
    shared_ptr<RelationalExpression> condExpr = dynamic_pointer_cast<RelationalExpression>(result->getConditionalExpression());
    REQUIRE(condExpr->getOperator() == RelationalOperator::LESS_THAN_OR_EQUALS);
    shared_ptr<NameExpression> nameExpr = dynamic_pointer_cast<NameExpression>(condExpr->getRelFactor1());
    REQUIRE(nameExpr->getVarName() == "x");
    shared_ptr<ConstantExpression> constExpr = dynamic_pointer_cast<ConstantExpression>(condExpr->getRelFactor2());
    REQUIRE(constExpr->getValue() == 5);
}

TEST_CASE("Parse statement") {
    string stmt1 = "print x";
    string stmt2 = "read x";
    string stmt3 = "if (!(x == y)) then {\n"
                   "\t\t\t\tread x;\n"
                   "\t\t} else {\n"
                   "\t\t\t\tprint x;\n"
                   "\t\t}";
    string stmt4 = "while (x >= y) {\n"
                   "\t\tprint x;\n"
                   "\t\tread x;\n"
                   "\t\t}\n";

    shared_ptr<PrintStatement> printStmt = dynamic_pointer_cast<PrintStatement>(Parser::parseStatement(stmt1));
    REQUIRE(printStmt->getVariableName() == "x");

    shared_ptr<ReadStatement> readStatement = dynamic_pointer_cast<ReadStatement>(Parser::parseStatement(stmt2));
    REQUIRE(readStatement->getVariableName() == "x");

    shared_ptr<IfStatement> ifStatement = dynamic_pointer_cast<IfStatement>(Parser::parseStatement(stmt3));
    shared_ptr<NotCondition> condExpr = dynamic_pointer_cast<NotCondition>(ifStatement->getConditionalExpression());
    shared_ptr<RelationalExpression> relExpr = dynamic_pointer_cast<RelationalExpression>(condExpr->getConditionalExpression());
    REQUIRE(relExpr->getOperator() == RelationalOperator::EQUALS);
    shared_ptr<NameExpression> nameExpr = dynamic_pointer_cast<NameExpression>(relExpr->getRelFactor1());
    REQUIRE(nameExpr->getVarName() == "x");
    nameExpr = dynamic_pointer_cast<NameExpression>(relExpr->getRelFactor2());
    REQUIRE(nameExpr->getVarName() == "y");
    readStatement = dynamic_pointer_cast<ReadStatement>((ifStatement->getThenStatements())[0]);
    REQUIRE(readStatement->getVariableName() == "x");
    printStmt = dynamic_pointer_cast<PrintStatement>((ifStatement->getElseStatements())[0]);
    REQUIRE(printStmt->getVariableName() == "x");

    shared_ptr<WhileStatement> whileStatement = dynamic_pointer_cast<WhileStatement>(Parser::parseStatement(stmt4));
    relExpr = dynamic_pointer_cast<RelationalExpression>(whileStatement->getConditionalExpression());
    REQUIRE(relExpr->getOperator() == RelationalOperator::GREATER_THAN_OR_EQUALS);
    nameExpr = dynamic_pointer_cast<NameExpression>(relExpr->getRelFactor1());
    REQUIRE(nameExpr->getVarName() == "x");
    nameExpr = dynamic_pointer_cast<NameExpression>(relExpr->getRelFactor2());
    REQUIRE(nameExpr->getVarName() == "y");
    printStmt = dynamic_pointer_cast<PrintStatement>((whileStatement->getStatements())[0]);
    REQUIRE(printStmt->getVariableName() == "x");
    readStatement = dynamic_pointer_cast<ReadStatement>((whileStatement->getStatements())[1]);
    REQUIRE(readStatement->getVariableName() == "x");
}

TEST_CASE("Extract Relational Conditional Expression") {
    string stmt = "while (1>= 1%((0-1)) )";
    string expected = "1>= 1%((0-1))";
    string result = Parser::extractConditionalExpr(stmt);
    REQUIRE(result == expected);
}

TEST_CASE("Parse complex relational expression conditional - 2") {
    string str = "1>= 1%((1))";
    shared_ptr<RelationalExpression> relExpr = Parser::parseRelExpr(str);
    REQUIRE(relExpr->getOperator() == RelationalOperator::GREATER_THAN_OR_EQUALS);
    shared_ptr<ConstantExpression> relFact1 = dynamic_pointer_cast<ConstantExpression>(relExpr->getRelFactor1());
    REQUIRE(relFact1->getValue() == 1);
    shared_ptr<OperatedExpression> opExpr = dynamic_pointer_cast<OperatedExpression>(relExpr->getRelFactor2());
    REQUIRE(opExpr->getOperator() == Operator::MODULO);
    shared_ptr<ConstantExpression> constExpr = dynamic_pointer_cast<ConstantExpression>(opExpr->getExpression1());
    REQUIRE(constExpr->getValue() == 1);
    shared_ptr<ConstantExpression> constExpr2 = dynamic_pointer_cast<ConstantExpression>(opExpr->getExpression2());
    REQUIRE(constExpr2->getValue() == 1);
}

TEST_CASE("Extract Relational Conditional Expression- 2") {
    string stmt = "while (1>= 1%((1)) )";
    string expected = "1>= 1%((1))";
    string result = Parser::extractConditionalExpr(stmt);
    REQUIRE(result == expected);
}

TEST_CASE("Extract Relational Conditional Expression- 3") {
    string stmt = "while (! ((1==0) && (1==0)))";
    string expected = "! ((1==0) && (1==0))";
    string result = Parser::extractConditionalExpr(stmt);
    REQUIRE(result == expected);
}

TEST_CASE("Parse complex conditional expression - 3") {
    string str = "! ((1==0) && (1==0))";
    shared_ptr<NotCondition> notCond = dynamic_pointer_cast<NotCondition>(Parser::parseCondExpr(str));
    shared_ptr<AndCondition> andCond = dynamic_pointer_cast<AndCondition>(notCond->getConditionalExpression());

    shared_ptr<RelationalExpression> relExpr1 = dynamic_pointer_cast<RelationalExpression>(andCond->getConditionalExpression1());
    REQUIRE(relExpr1->getOperator() == RelationalOperator::EQUALS);
    shared_ptr<ConstantExpression> constExpr1 = dynamic_pointer_cast<ConstantExpression>(relExpr1->getRelFactor1());
    REQUIRE(constExpr1->getValue() == 1);
    shared_ptr<ConstantExpression> constExpr2 = dynamic_pointer_cast<ConstantExpression>(relExpr1->getRelFactor2());
    REQUIRE(constExpr2->getValue() == 0);

    shared_ptr<RelationalExpression> relExpr2 = dynamic_pointer_cast<RelationalExpression>(andCond->getConditionalExpression2());
    REQUIRE(relExpr1->getOperator() == RelationalOperator::EQUALS);
    constExpr1 = dynamic_pointer_cast<ConstantExpression>(relExpr2->getRelFactor1());
    REQUIRE(constExpr1->getValue() == 1);
    constExpr2 = dynamic_pointer_cast<ConstantExpression>(relExpr2->getRelFactor2());
    REQUIRE(constExpr2->getValue() == 0);
}

TEST_CASE("Parse complex conditional -4") {
    string source6 = "(1 >= x ) || (!(procedure == 2147483647))";
    shared_ptr<OrCondition> orCondition = dynamic_pointer_cast<OrCondition>(Parser::parseCondExpr(source6));
    shared_ptr<RelationalExpression> relExpr1 = dynamic_pointer_cast<RelationalExpression>(orCondition->getConditionalExpression1());
    REQUIRE(relExpr1->getOperator() == RelationalOperator::GREATER_THAN_OR_EQUALS);
    shared_ptr<ConstantExpression> constExpr = dynamic_pointer_cast<ConstantExpression>(relExpr1->getRelFactor1());
    shared_ptr<NameExpression> nameExpr = dynamic_pointer_cast<NameExpression>(relExpr1->getRelFactor2());
    REQUIRE(constExpr->getValue() == 1);
    REQUIRE(nameExpr->getVarName() == "x");
    shared_ptr<NotCondition> notCondition = dynamic_pointer_cast<NotCondition>(orCondition->getConditionalExpression2());
    shared_ptr<RelationalExpression> relExpr2 = dynamic_pointer_cast<RelationalExpression>(notCondition->getConditionalExpression());
    nameExpr = dynamic_pointer_cast<NameExpression>(relExpr2->getRelFactor1());
    REQUIRE(nameExpr->getVarName() == "procedure");
    constExpr = dynamic_pointer_cast<ConstantExpression>(relExpr2->getRelFactor2());
    REQUIRE(constExpr->getValue() == 2147483647);
}

TEST_CASE("Syntax Error for missing else block for if statement") {
    string str = "if (!(x == y)) then {\n"
                 "\t\t\t\tread x;\n"
                 "\t\t}";
    REQUIRE_THROWS_AS(Parser::parseStatement(str), InvalidSyntaxException);
}

TEST_CASE("Syntax Error for missing then keyword for if statement") {
    string str = "if (!(x == y)) {\n"
                 "\t\t\t\tread x;\n"
                 "\t\t}";
    REQUIRE_THROWS_AS(Parser::parseStatement(str), InvalidSyntaxException);
}

TEST_CASE("Syntax Error for invalid conditional, missing condition") {
    string str = "while (!(x == y) && ) {\n"
                 "\t\t\t\tread x;\n"
                 "\t\t}";
    REQUIRE_THROWS_AS(Parser::parseStatement(str), InvalidSyntaxException);
}

TEST_CASE("Syntax Error for invalid conditional, extra bracket") {
    string str = "while (!(x == y))) {\n"
                 "\t\t\t\tread x;\n"
                 "\t\t}";
    REQUIRE_THROWS_AS(Parser::parseStatement(str), InvalidSyntaxException);
}

TEST_CASE("Syntax Error for invalid statement, missing semicolon") {
    string str = "procedure ABC {\n"
                 "\t\t\t\tread x\n"
                 "\t\t}";
    vector<string> stmtLst;
    REQUIRE_THROWS_AS(Parser::extractStatements(str, stmtLst), InvalidSyntaxException);
}

TEST_CASE("Syntax Error for invalid nested statement, missing semicolon") {
    string str = "while (x == 0) {\n"
                 "\t\t\t\tread x\n"
                 "\t\t}";
    REQUIRE_THROWS_AS(Parser::parseWhile(str), InvalidSyntaxException);
}

TEST_CASE("Syntax Error for invalid syntax, extra } bracket") {
    string str = "while (!(x == y)) {\n"
                 "\t\t\t\tread x;\n"
                 "\t\t}}";
    REQUIRE_THROWS_AS(Parser::parseStatement(str), InvalidSyntaxException);
}

TEST_CASE("Syntax Error for invalid syntax, extra { bracket") {
    string str = "while (!(x == y)) {{\n"
                 "\t\t\t\tread x;\n"
                 "\t\t}";
    REQUIRE_THROWS_AS(Parser::parseStatement(str), InvalidSyntaxException);
}

TEST_CASE("Syntax Error for invalid syntax, missing } bracket") {
    string str = "while (x<1) {\n"
                 "\t\t\t\tread x;\n";
    REQUIRE_THROWS_AS(Parser::parseStatement(str), InvalidSyntaxException);
}

TEST_CASE("Syntax Error for invalid syntax, missing { bracket") {
    string str = "while (x<1) \n"
                 "\t\t\t\tread x;}\n";
    REQUIRE_THROWS_AS(Parser::parseStatement(str), InvalidSyntaxException);
}

TEST_CASE("Syntax Error for invalid syntax, missing { bracket in if stmt") {
    string str = "if (x>y) then \n"
                 "\t\t\t\tread x;}\n";
    REQUIRE_THROWS_AS(Parser::parseStatement(str), InvalidSyntaxException);
}

TEST_CASE("Syntax Error for invalid syntax, missing { bracket in while stmt") {
    string str = "while (x>y) \n"
                 "\t\t\t\tread x;}\n";
    REQUIRE_THROWS_AS(Parser::parseStatement(str), InvalidSyntaxException);
}

TEST_CASE("Nested if block") {
    string str = "if (x > 3) then {\n"
                 "if (x > 3 )then {\n"
                 "x=1;\n"
                 "}\n"
                 "else{\n"
                 "x=1;\n"
                 "}\n"
                 "}\n"
                 "else{\n"
                 "x=1;\n"
                 "}\n";
    shared_ptr<IfStatement> ifStatement = Parser::parseIfElse(str);
    REQUIRE_NOTHROW(Parser::parseIfElse(str));
    shared_ptr<RelationalExpression> relationalExpression = dynamic_pointer_cast<RelationalExpression>(ifStatement->getConditionalExpression());
    REQUIRE(relationalExpression->getOperator() == RelationalOperator::GREATER_THAN);
    shared_ptr<NameExpression> nameExpression = dynamic_pointer_cast<NameExpression>(relationalExpression->getRelFactor1());
    REQUIRE(nameExpression->getVarName() == "x");
    shared_ptr<ConstantExpression> constantExpression = dynamic_pointer_cast<ConstantExpression>(relationalExpression->getRelFactor2());
    REQUIRE(constantExpression->getValue() == 3);

    shared_ptr<IfStatement> innerIfStatement = dynamic_pointer_cast<IfStatement>(ifStatement->getThenStatements()[0]);
    relationalExpression = dynamic_pointer_cast<RelationalExpression>(innerIfStatement->getConditionalExpression());
    REQUIRE(relationalExpression->getOperator() == RelationalOperator::GREATER_THAN);
    nameExpression = dynamic_pointer_cast<NameExpression>(relationalExpression->getRelFactor1());
    REQUIRE(nameExpression->getVarName() == "x");
    constantExpression = dynamic_pointer_cast<ConstantExpression>(relationalExpression->getRelFactor2());
    REQUIRE(constantExpression->getValue() == 3);
    shared_ptr<AssignStatement> thenStatement = dynamic_pointer_cast<AssignStatement>(innerIfStatement->getThenStatements()[0]);
    REQUIRE(thenStatement->getVarName() == "x");
    constantExpression = dynamic_pointer_cast<ConstantExpression>(thenStatement->getRelFactor());
    REQUIRE(constantExpression->getValue() == 1);
    shared_ptr<AssignStatement> elseStatement = dynamic_pointer_cast<AssignStatement>(innerIfStatement->getElseStatements()[0]);
    REQUIRE(elseStatement->getVarName() == "x");
    constantExpression = dynamic_pointer_cast<ConstantExpression>(elseStatement->getRelFactor());
    REQUIRE(constantExpression->getValue() == 1);

    elseStatement = dynamic_pointer_cast<AssignStatement>(ifStatement->getElseStatements()[0]);
    REQUIRE(elseStatement->getVarName() == "x");
    constantExpression = dynamic_pointer_cast<ConstantExpression>(elseStatement->getRelFactor());
    REQUIRE(constantExpression->getValue() == 1);
}

TEST_CASE("Parse if else statement") {
    string stmt3 = "if (!(x == y)) then {\n"
                   "\t\t\t\tread x;\n"
                   "\t\t} else {\n"
                   "\t\t\t\tprint x;\n"
                   "\t\t}";

    shared_ptr<IfStatement> ifStatement = dynamic_pointer_cast<IfStatement>(Parser::parseStatement(stmt3));
    shared_ptr<NotCondition> condExpr = dynamic_pointer_cast<NotCondition>(ifStatement->getConditionalExpression());
    shared_ptr<RelationalExpression> relExpr = dynamic_pointer_cast<RelationalExpression>(condExpr->getConditionalExpression());
    REQUIRE(relExpr->getOperator() == RelationalOperator::EQUALS);
    shared_ptr<NameExpression> nameExpr = dynamic_pointer_cast<NameExpression>(relExpr->getRelFactor1());
    REQUIRE(nameExpr->getVarName() == "x");
    nameExpr = dynamic_pointer_cast<NameExpression>(relExpr->getRelFactor2());
    REQUIRE(nameExpr->getVarName() == "y");
    shared_ptr<ReadStatement> readStatement = dynamic_pointer_cast<ReadStatement>((ifStatement->getThenStatements())[0]);
    REQUIRE(readStatement->getVariableName() == "x");
    shared_ptr<PrintStatement> printStmt = dynamic_pointer_cast<PrintStatement>((ifStatement->getElseStatements())[0]);
    REQUIRE(printStmt->getVariableName() == "x");
}

TEST_CASE("Parse procedure with nested if else") {
    string str = "procedure Main {\n"
                 "if (x > 3) then {\n"
                 "if (x > 3 )then {\n"
                 "x=1;\n"
                 "}\n"
                 "else{\n"
                 "x=1;\n"
                 "}\n"
                 "}\n"
                 "else{\n"
                 "x=1;\n"
                 "}\n"
                 "}";
    shared_ptr<Procedure> procedure = Parser::parseProcedure(str);
    REQUIRE(procedure->getProcedureName() == "Main");
    shared_ptr<IfStatement> ifStatement = dynamic_pointer_cast<IfStatement>(procedure->getStatements()[0]);
    shared_ptr<RelationalExpression> relationalExpression = dynamic_pointer_cast<RelationalExpression>(ifStatement->getConditionalExpression());
    REQUIRE(relationalExpression->getOperator() == RelationalOperator::GREATER_THAN);
    shared_ptr<NameExpression> nameExpression = dynamic_pointer_cast<NameExpression>(relationalExpression->getRelFactor1());
    REQUIRE(nameExpression->getVarName() == "x");
    shared_ptr<ConstantExpression> constantExpression = dynamic_pointer_cast<ConstantExpression>(relationalExpression->getRelFactor2());
    REQUIRE(constantExpression->getValue() == 3);

    shared_ptr<IfStatement> innerIfStatement = dynamic_pointer_cast<IfStatement>(ifStatement->getThenStatements()[0]);
    relationalExpression = dynamic_pointer_cast<RelationalExpression>(innerIfStatement->getConditionalExpression());
    REQUIRE(relationalExpression->getOperator() == RelationalOperator::GREATER_THAN);
    nameExpression = dynamic_pointer_cast<NameExpression>(relationalExpression->getRelFactor1());
    REQUIRE(nameExpression->getVarName() == "x");
    constantExpression = dynamic_pointer_cast<ConstantExpression>(relationalExpression->getRelFactor2());
    REQUIRE(constantExpression->getValue() == 3);
    shared_ptr<AssignStatement> thenStatement = dynamic_pointer_cast<AssignStatement>(innerIfStatement->getThenStatements()[0]);
    REQUIRE(thenStatement->getVarName() == "x");
    constantExpression = dynamic_pointer_cast<ConstantExpression>(thenStatement->getRelFactor());
    REQUIRE(constantExpression->getValue() == 1);
    shared_ptr<AssignStatement> elseStatement = dynamic_pointer_cast<AssignStatement>(innerIfStatement->getElseStatements()[0]);
    REQUIRE(elseStatement->getVarName() == "x");
    constantExpression = dynamic_pointer_cast<ConstantExpression>(elseStatement->getRelFactor());
    REQUIRE(constantExpression->getValue() == 1);

    elseStatement = dynamic_pointer_cast<AssignStatement>(ifStatement->getElseStatements()[0]);
    REQUIRE(elseStatement->getVarName() == "x");
    constantExpression = dynamic_pointer_cast<ConstantExpression>(elseStatement->getRelFactor());
    REQUIRE(constantExpression->getValue() == 1);
}

TEST_CASE("Tokenize Call - 1") {
    string rawCallStatement = "call monster";
    shared_ptr<CallStatement> callStatement = Tokenizer::tokenizeCall(rawCallStatement);
    REQUIRE(callStatement->getProcedureName() == "monster");
}

TEST_CASE("RelationalFactor Generate String") {
    string rawRelFactor = "a%b-c/d+e%f*g+h-i";
    shared_ptr<RelationalFactor> relFactor = Tokenizer::tokenizeRelFactor(rawRelFactor);
    REQUIRE(relFactor->generateString() == "(((((a % b) - (c / d)) + ((e % f) * g)) + h) - i)");

    rawRelFactor = "(a + b) * (3 + (x * 3 * 2 % ((1-2) / q)) + 9)";
    relFactor = Tokenizer::tokenizeRelFactor(rawRelFactor);
    REQUIRE(relFactor->generateString() == "((a + b) * ((3 + (((x * 3) * 2) % ((1 - 2) / q))) + 9))");
}
