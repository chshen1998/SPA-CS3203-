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
    shared_ptr<SourceCode> sourceCode = make_shared<SourceCode>("");
    shared_ptr<Procedure> procedure1 = make_shared<Procedure>(sourceCode, "p1");
    shared_ptr<ReadStatement> readStatement = make_shared<ReadStatement>(procedure1, "v");

    string line = "read v";
    shared_ptr<ReadStatement> result = Tokenizer::tokenizeRead(line, procedure1);

    REQUIRE(result->getVariableName() == readStatement->getVariableName());
    REQUIRE(result->getParent() == readStatement->getParent());
}

TEST_CASE("Tokenize print") {
    shared_ptr<SourceCode> sourceCode = make_shared<SourceCode>("");
    shared_ptr<Procedure> procedure1 = make_shared<Procedure>(sourceCode, "p1");
    shared_ptr<PrintStatement> printStatement = make_shared<PrintStatement>(procedure1, "v");

    string line = "print v";
    shared_ptr<PrintStatement> result = Tokenizer::tokenizePrint(line, procedure1);

    REQUIRE(result->getVariableName() == printStatement->getVariableName());
    REQUIRE(result->getParent() == printStatement->getParent());
}

TEST_CASE("Tokenize Statements") {
    shared_ptr<SourceCode> sourceCode = make_shared<SourceCode>("");
    shared_ptr<Procedure> procedure1 = make_shared<Procedure>(sourceCode, "p1");

    vector<shared_ptr<Procedure> > procedures;
    procedures.push_back(procedure1);

    vector<vector<string> > statements;
    vector<string> stmts1;

    string s1 = "print flag";
    string s2 = "read flag";

    stmts1.push_back(s1);
    stmts1.push_back(s2);

    statements.push_back(stmts1);

    vector<shared_ptr<Procedure> > result = Tokenizer::tokenizeStatements(procedures, statements);

    vector<shared_ptr<Procedure> > expected;
    shared_ptr<PrintStatement> print1 = make_shared<PrintStatement>(procedure1, "flag");
    shared_ptr<ReadStatement> read1 = make_shared<ReadStatement>(procedure1, "flag");

    vector<shared_ptr<Statement> > resultStatements = result[0]->getStatements();

    REQUIRE(resultStatements[0]->getParent() == print1->getParent());

    REQUIRE(resultStatements[1]->getParent() == read1->getParent());

}

TEST_CASE("Tokenize SourceCode") {
    shared_ptr<SourceCode> srcCode = make_shared<SourceCode>("");

    vector<string> names;
    string name = "ABC";
    names.push_back(name);
    shared_ptr<Procedure> p1 = make_shared<Procedure>(srcCode, name);

    vector<vector<string> > statements;
    string s1 = "print flag";
    string s2 = "read flag";
    vector<string> stmtlst;
    stmtlst.push_back(s1);
    stmtlst.push_back(s2);
    statements.push_back(stmtlst);

    shared_ptr<PrintStatement> print1 = make_shared<PrintStatement>(p1, "flag");
    shared_ptr<ReadStatement> read1 = make_shared<ReadStatement>(p1, "flag");

    p1->addStatement(print1);
    p1->addStatement(read1);
    srcCode->addProcedure(p1);

    shared_ptr<SourceCode> srcCode2 = make_shared<SourceCode>("");
    srcCode2 = Tokenizer::tokenize(srcCode, names, statements);

    vector<shared_ptr<Procedure> > resultProcLst = srcCode2->getProcedures();
    vector<shared_ptr<Statement> > resultStmtLst = resultProcLst[0]->getStatements();
    string resultProcName = resultProcLst[0]->getProcedureName();

    REQUIRE(resultStmtLst[0]->getLineNum() == print1->getLineNum());
    REQUIRE(resultStmtLst[0]->getParent() == print1->getParent());

    REQUIRE(resultStmtLst[1]->getLineNum() == read1->getLineNum());
    REQUIRE(resultStmtLst[1]->getParent() == read1->getParent());

}

TEST_CASE("Tokenize RelFactor - Simple Addition / Subtraction") {
    string rawAssignStatement = "a - 1 + hmm - 9312";
    shared_ptr<RelationalFactor> relFactor = Tokenizer::tokenizeRelFactor(rawAssignStatement);

    shared_ptr<NameExpression> nameExpression;
    shared_ptr<ConstantExpression> constantExpression;
    shared_ptr<OperatedExpression> operatedExpression;

    operatedExpression = dynamic_pointer_cast<OperatedExpression>(relFactor);
    nameExpression = dynamic_pointer_cast<NameExpression>(operatedExpression->getExpression1());
    REQUIRE(operatedExpression->getOperator() == Operator::SUBTRACT);
    REQUIRE(nameExpression->getVarName() == "a");

    operatedExpression = dynamic_pointer_cast<OperatedExpression>(operatedExpression->getExpression2());
    constantExpression = dynamic_pointer_cast<ConstantExpression>(operatedExpression->getExpression1());
    REQUIRE(operatedExpression->getOperator() == Operator::ADD);
    REQUIRE(constantExpression->getValue() == 1);

    operatedExpression = dynamic_pointer_cast<OperatedExpression>(operatedExpression->getExpression2());
    nameExpression = dynamic_pointer_cast<NameExpression>(operatedExpression->getExpression1());
    REQUIRE(operatedExpression->getOperator() == Operator::SUBTRACT);
    REQUIRE(nameExpression->getVarName() == "hmm");

    constantExpression = dynamic_pointer_cast<ConstantExpression>(operatedExpression->getExpression2());

    REQUIRE(constantExpression->getValue() == 9312);
}

TEST_CASE("Tokenize RelFactor - Simple Multiplication / Division / Modulo") {
    string rawAssignStatement = "a % 1 / hmm * 9312";
    shared_ptr<RelationalFactor> relFactor = Tokenizer::tokenizeRelFactor(rawAssignStatement);

    shared_ptr<NameExpression> nameExpression;
    shared_ptr<ConstantExpression> constantExpression;
    shared_ptr<OperatedExpression> operatedExpression;

    operatedExpression = dynamic_pointer_cast<OperatedExpression>(relFactor);
    nameExpression = dynamic_pointer_cast<NameExpression>(operatedExpression->getExpression1());
    REQUIRE(operatedExpression->getOperator() == Operator::MODULO);
    REQUIRE(nameExpression->getVarName() == "a");

    operatedExpression = dynamic_pointer_cast<OperatedExpression>(operatedExpression->getExpression2());
    constantExpression = dynamic_pointer_cast<ConstantExpression>(operatedExpression->getExpression1());
    REQUIRE(operatedExpression->getOperator() == Operator::DIVIDE);
    REQUIRE(constantExpression->getValue() == 1);

    operatedExpression = dynamic_pointer_cast<OperatedExpression>(operatedExpression->getExpression2());
    nameExpression = dynamic_pointer_cast<NameExpression>(operatedExpression->getExpression1());
    REQUIRE(operatedExpression->getOperator() == Operator::MULTIPLY);
    REQUIRE(nameExpression->getVarName() == "hmm");

    constantExpression = dynamic_pointer_cast<ConstantExpression>(operatedExpression->getExpression2());

    REQUIRE(constantExpression->getValue() == 9312);
}

TEST_CASE("Tokenize RelFactor - Simple, ALL Operators") {
    string rawAssignStatement = "a % b - c / d + e % f * g + h - i";
    shared_ptr<RelationalFactor> relFactor = Tokenizer::tokenizeRelFactor(rawAssignStatement);

    shared_ptr<NameExpression> nameExpression;
    shared_ptr<OperatedExpression> operatedExpression;

    operatedExpression = dynamic_pointer_cast<OperatedExpression>(relFactor);
    REQUIRE(operatedExpression->getOperator() == Operator::SUBTRACT);

    // a % b
    operatedExpression = dynamic_pointer_cast<OperatedExpression>(operatedExpression->getExpression1());
    REQUIRE(operatedExpression->getOperator() == Operator::MODULO);
    nameExpression = dynamic_pointer_cast<NameExpression>(operatedExpression->getExpression1());
    REQUIRE(nameExpression->getVarName() == "a");
    nameExpression = dynamic_pointer_cast<NameExpression>(operatedExpression->getExpression2());
    REQUIRE(nameExpression->getVarName() == "b");

    // c / d   +   e % f * g + h - i
    operatedExpression = dynamic_pointer_cast<OperatedExpression>(relFactor);
    operatedExpression = dynamic_pointer_cast<OperatedExpression>(operatedExpression->getExpression2());
    REQUIRE(operatedExpression->getOperator() == Operator::ADD);

    // c / d
    operatedExpression = dynamic_pointer_cast<OperatedExpression>(operatedExpression->getExpression1());
    REQUIRE(operatedExpression->getOperator() == Operator::DIVIDE);
    nameExpression = dynamic_pointer_cast<NameExpression>(operatedExpression->getExpression1());
    REQUIRE(nameExpression->getVarName() == "c");
    nameExpression = dynamic_pointer_cast<NameExpression>(operatedExpression->getExpression2());
    REQUIRE(nameExpression->getVarName() == "d");


    // e % f * g + h - i
    operatedExpression = dynamic_pointer_cast<OperatedExpression>(relFactor);
    operatedExpression = dynamic_pointer_cast<OperatedExpression>(operatedExpression->getExpression2());
    shared_ptr<OperatedExpression> savePoint;
    savePoint = dynamic_pointer_cast<OperatedExpression>(operatedExpression->getExpression2());
    REQUIRE(savePoint->getOperator() == Operator::ADD);

    // e % f * g
    operatedExpression = dynamic_pointer_cast<OperatedExpression>(savePoint->getExpression1());
    REQUIRE(operatedExpression->getOperator() == Operator::MODULO);
    nameExpression = dynamic_pointer_cast<NameExpression>(operatedExpression->getExpression1());
    REQUIRE(nameExpression->getVarName() == "e");

    // f * g
    operatedExpression = dynamic_pointer_cast<OperatedExpression>(operatedExpression->getExpression2());
    REQUIRE(operatedExpression->getOperator() == Operator::MULTIPLY);
    nameExpression = dynamic_pointer_cast<NameExpression>(operatedExpression->getExpression1());
    REQUIRE(nameExpression->getVarName() == "f");
    nameExpression = dynamic_pointer_cast<NameExpression>(operatedExpression->getExpression2());
    REQUIRE(nameExpression->getVarName() == "g");

    // h - i
    operatedExpression = dynamic_pointer_cast<OperatedExpression>(savePoint->getExpression2());
    REQUIRE(operatedExpression->getOperator() == Operator::SUBTRACT);
    nameExpression = dynamic_pointer_cast<NameExpression>(operatedExpression->getExpression1());
    REQUIRE(nameExpression->getVarName() == "h");
    nameExpression = dynamic_pointer_cast<NameExpression>(operatedExpression->getExpression2());
    REQUIRE(nameExpression->getVarName() == "i");
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
    string rawAssignStatement = "(((a + b) + c) + d)";
    shared_ptr<RelationalFactor> relFactor = Tokenizer::tokenizeRelFactor(rawAssignStatement);

    shared_ptr<NameExpression> nameExpression;
    shared_ptr<OperatedExpression> operatedExpression;

    operatedExpression = dynamic_pointer_cast<OperatedExpression>(relFactor);
    REQUIRE(operatedExpression->getOperator() == Operator::ADD);
    nameExpression = dynamic_pointer_cast<NameExpression>(operatedExpression->getExpression2());
    REQUIRE(nameExpression->getVarName() == "d");

    operatedExpression = dynamic_pointer_cast<OperatedExpression>(operatedExpression->getExpression1());
    REQUIRE(operatedExpression->getOperator() == Operator::ADD);
    nameExpression = dynamic_pointer_cast<NameExpression>(operatedExpression->getExpression2());
    REQUIRE(nameExpression->getVarName() == "c");

    operatedExpression = dynamic_pointer_cast<OperatedExpression>(operatedExpression->getExpression1());
    REQUIRE(operatedExpression->getOperator() == Operator::ADD);
    nameExpression = dynamic_pointer_cast<NameExpression>(operatedExpression->getExpression2());
    REQUIRE(nameExpression->getVarName() == "b");
    nameExpression = dynamic_pointer_cast<NameExpression>(operatedExpression->getExpression1());
    REQUIRE(nameExpression->getVarName() == "a");
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

TEST_CASE("Extract Statement Block") {
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
    size_t firstEgyptianOpen = stmt1.find_first_of("{");
    string result = Parser::extractStatementBlock(stmt1, firstEgyptianOpen);
    string expected = "call x;\n"
                      "\t\tread x;\n"
                      "\t\twhile () {\n"
                      "\t\t\t\tread x;\n"
                      "\t\t}\n"
                      "\t\tif () then {\n"
                      "\t\t\t\tread x;\n"
                      "\t\t} else {\n"
                      "\t\t\t\tread x;\n"
                      "\t\t}";
    REQUIRE(expected == result);
}

TEST_CASE("Parse relational expression") {
    string expr1 = "x >= 5";
    shared_ptr<RelationalExpression> relExpr1 = Parser::parseRelExpr(expr1, nullptr);
    shared_ptr<NameExpression> nameExpr = dynamic_pointer_cast<NameExpression>(relExpr1->getRelFactor1());
    shared_ptr<ConstantExpression> constExpr = dynamic_pointer_cast<ConstantExpression>(relExpr1->getRelFactor2());
    REQUIRE(relExpr1->getOperator() == RelationalOperator::GREATER_THAN_OR_EQUALS);
    REQUIRE(nameExpr->getVarName() == "x");
    REQUIRE(constExpr->getValue() == 5);

    string expr2 = "x > y";
    shared_ptr<RelationalExpression> relExpr2 = Parser::parseRelExpr(expr2, nullptr);
    REQUIRE(relExpr2->getOperator() == RelationalOperator::GREATER_THAN);
    nameExpr = dynamic_pointer_cast<NameExpression>(relExpr2->getRelFactor1());
    REQUIRE(nameExpr->getVarName() == "x");
    nameExpr = dynamic_pointer_cast<NameExpression>(relExpr2->getRelFactor2());
    REQUIRE(nameExpr->getVarName() == "y");

    string expr3 = "x == 5";
    shared_ptr<RelationalExpression> relExpr3 = Parser::parseRelExpr(expr3, nullptr);
    REQUIRE(relExpr3->getOperator() == RelationalOperator::EQUALS);
    nameExpr = dynamic_pointer_cast<NameExpression>(relExpr3->getRelFactor1());
    constExpr = dynamic_pointer_cast<ConstantExpression>(relExpr3->getRelFactor2());
    REQUIRE(nameExpr->getVarName() == "x");
    REQUIRE(constExpr->getValue() == 5);

    string expr4 = "x != (y + 5)";
    shared_ptr<RelationalExpression> relExpr4 = Parser::parseRelExpr(expr4, nullptr);
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
    shared_ptr<RelationalExpression> result = dynamic_pointer_cast<RelationalExpression>(Parser::parseCondExpr(str, nullptr));
    REQUIRE(result->getOperator() == RelationalOperator::LESS_THAN_OR_EQUALS);
    shared_ptr<NameExpression> nameExpr = dynamic_pointer_cast<NameExpression>(result->getRelFactor1());
    REQUIRE(nameExpr->getVarName() == "x");
    shared_ptr<ConstantExpression> constExpr = dynamic_pointer_cast<ConstantExpression>(result->getRelFactor2());
    REQUIRE(constExpr->getValue() == 5);
}

TEST_CASE("Parse Conditional Expression - Relational Condition with Extra bracket") {
    string str = "(x <= 5)";
    shared_ptr<RelationalExpression> result = dynamic_pointer_cast<RelationalExpression>(Parser::parseCondExpr(str, nullptr));
    REQUIRE(result->getOperator() == RelationalOperator::LESS_THAN_OR_EQUALS);
    shared_ptr<NameExpression> nameExpr = dynamic_pointer_cast<NameExpression>(result->getRelFactor1());
    REQUIRE(nameExpr->getVarName() == "x");
    shared_ptr<ConstantExpression> constExpr = dynamic_pointer_cast<ConstantExpression>(result->getRelFactor2());
    REQUIRE(constExpr->getValue() == 5);
}

TEST_CASE("Parse Conditional Expression - Not Condition with Relational Expression") {
    string str = "!(x <= 5)";
    shared_ptr<NotCondition> result = dynamic_pointer_cast<NotCondition>(Parser::parseCondExpr(str, nullptr));
    shared_ptr<RelationalExpression> condExpr = dynamic_pointer_cast<RelationalExpression>(result->getConditionalExpression());
    REQUIRE(condExpr->getOperator() == RelationalOperator::LESS_THAN_OR_EQUALS);
    shared_ptr<NameExpression> nameExpr = dynamic_pointer_cast<NameExpression>(condExpr->getRelFactor1());
    REQUIRE(nameExpr->getVarName() == "x");
    shared_ptr<ConstantExpression> constExpr = dynamic_pointer_cast<ConstantExpression>(condExpr->getRelFactor2());
    REQUIRE(constExpr->getValue() == 5);
}

TEST_CASE("Parse Conditional Expression - Not Condition with nested Or conditional expression") {
    string str = "!((x <= 5) || (x == 10))";
    shared_ptr<NotCondition> result = dynamic_pointer_cast<NotCondition>(Parser::parseCondExpr(str, nullptr));
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
    shared_ptr<AndCondition> result = dynamic_pointer_cast<AndCondition>(Parser::parseCondExpr(str, nullptr));
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
    shared_ptr<OrCondition> orCond = dynamic_pointer_cast<OrCondition>(Parser::parseCondExpr(str, nullptr));
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

    shared_ptr<PrintStatement> printStmt = dynamic_pointer_cast<PrintStatement>(Parser::parseStatement(stmt1, nullptr));
    REQUIRE(printStmt->getVariableName() == "x");

    shared_ptr<ReadStatement> readStatement = dynamic_pointer_cast<ReadStatement>(Parser::parseStatement(stmt2, nullptr));
    REQUIRE(readStatement->getVariableName() == "x");

    shared_ptr<IfStatement> ifStatement = dynamic_pointer_cast<IfStatement>(Parser::parseStatement(stmt3, nullptr));
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

    shared_ptr<WhileStatement> whileStatement = dynamic_pointer_cast<WhileStatement>(Parser::parseStatement(stmt4, nullptr));
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
    shared_ptr<RelationalExpression> relExpr = Parser::parseRelExpr(str, nullptr);
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
    shared_ptr<NotCondition> notCond = dynamic_pointer_cast<NotCondition>(Parser::parseCondExpr(str, nullptr));
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

TEST_CASE("Syntax Error for missing else block for if statement") {
    string str = "if (!(x == y)) then {\n"
                 "\t\t\t\tread x;\n"
                 "\t\t}";
    REQUIRE_THROWS_AS(Parser::parseStatement(str, nullptr), InvalidSyntaxException);
}

TEST_CASE("Syntax Error for missing then keyword for if statement") {
    string str = "if (!(x == y)) {\n"
                 "\t\t\t\tread x;\n"
                 "\t\t}";
    REQUIRE_THROWS_AS(Parser::parseStatement(str, nullptr), InvalidSyntaxException);
}

TEST_CASE("Syntax Error for invalid conditional, missing condition") {
    string str = "while (!(x == y) && ) {\n"
                 "\t\t\t\tread x;\n"
                 "\t\t}";
    REQUIRE_THROWS_AS(Parser::parseStatement(str, nullptr), InvalidSyntaxException);
}

TEST_CASE("Syntax Error for invalid conditional, extra bracket") {
    string str = "while (!(x == y))) {\n"
                 "\t\t\t\tread x;\n"
                 "\t\t}";
    REQUIRE_THROWS_AS(Parser::parseStatement(str, nullptr), InvalidSyntaxException);
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
    REQUIRE_THROWS_AS(Parser::parseWhile(str, nullptr), InvalidSyntaxException);
}

TEST_CASE("Syntax Error for invalid syntax, extra } bracket") {
    string str = "while (!(x == y)) {\n"
                 "\t\t\t\tread x;\n"
                 "\t\t}}";
    REQUIRE_THROWS_AS(Parser::parseStatement(str, nullptr), InvalidSyntaxException);
}

TEST_CASE("Syntax Error for invalid syntax, extra { bracket") {
    string str = "while (!(x == y)) {{\n"
                 "\t\t\t\tread x;\n" //takes statement as {read x;
                 "\t\t}";
    REQUIRE_THROWS_AS(Parser::parseStatement(str, nullptr), InvalidSyntaxException);
}

TEST_CASE("Syntax Error for invalid syntax, missing } bracket") {
    string str = "while (x<1) {\n"
                 "\t\t\t\tread x;\n";
    REQUIRE_THROWS_AS(Parser::parseStatement(str, nullptr), InvalidSyntaxException);
}

TEST_CASE("Syntax Error for invalid syntax, missing { bracket in if stmt") {
    string str = "if (x>y) then \n"
                 "\t\t\t\tread x;}\n";
    REQUIRE_THROWS_AS(Parser::parseStatement(str, nullptr), InvalidSyntaxException);
}

TEST_CASE("Syntax Error for invalid syntax, missing { bracket in while stmt") {
    string str = "while (x>y) \n"
                 "\t\t\t\tread x;}\n";
    REQUIRE_THROWS_AS(Parser::parseStatement(str, nullptr), InvalidSyntaxException);
}

TEST_CASE("Parse if-else") {
    string stmt3 = "if (!(x == y)) then {\n"
                   "\t\t\t\tread x;\n"
                   "\t\t} else {\n"
                   "\t\t\t\tprint x;\n"
                   "\t\t}";

    shared_ptr<IfStatement> ifStatement = dynamic_pointer_cast<IfStatement>(Parser::parseStatement(stmt3, nullptr));
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