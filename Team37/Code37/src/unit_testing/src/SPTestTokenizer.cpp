#include <catch.hpp>
#include <string>
#include <vector>

#include "SP/Tokenizer.h"
#include "SP/InvalidSyntaxException.h"

using namespace std;

/*
 * Naming Convention:
 * functionName - Positive/Negative Case - (Opt: Others)
 *
 * E.g.
 * tokenizeRelFactor - Negative Case - Very Nested Blocks
 */

TEST_CASE("tokenizeRead - Positive Case") {
    string rawReadStatement;
    shared_ptr<ReadStatement> result;

    rawReadStatement = "read v";
    result = Tokenizer::tokenizeRead(rawReadStatement);
    REQUIRE(result->getVariableName() == "v");

    rawReadStatement = "read variableWithNumbers12345";
    result = Tokenizer::tokenizeRead(rawReadStatement);
    REQUIRE(result->getVariableName() == "variableWithNumbers12345");

    rawReadStatement = "read A0123456R";
    result = Tokenizer::tokenizeRead(rawReadStatement);
    REQUIRE(result->getVariableName() == "A0123456R");

    rawReadStatement = "read VeryLongVeryLongVeryLongVeryLongVeryLongVeryLongVeryLongVeryLongVeryLongVeryLongVeryLongVeryLong";
    result = Tokenizer::tokenizeRead(rawReadStatement);
    REQUIRE(result->getVariableName() == "VeryLongVeryLongVeryLongVeryLongVeryLongVeryLongVeryLongVeryLongVeryLongVeryLongVeryLongVeryLong");
}

TEST_CASE("tokenizeRead - Negative Case") {
    string rawReadStatement;

    rawReadStatement = "read 999";
    REQUIRE_THROWS_AS(Tokenizer::tokenizeRead(rawReadStatement), InvalidSyntaxException);

    rawReadStatement = "read brackets)";
    REQUIRE_THROWS_AS(Tokenizer::tokenizeRead(rawReadStatement), InvalidSyntaxException);

    rawReadStatement = "read not(GonnaWork";
    REQUIRE_THROWS_AS(Tokenizer::tokenizeRead(rawReadStatement), InvalidSyntaxException);

    rawReadStatement = "read cmon...";
    REQUIRE_THROWS_AS(Tokenizer::tokenizeRead(rawReadStatement), InvalidSyntaxException);

    rawReadStatement = "read Invalid?";
    REQUIRE_THROWS_AS(Tokenizer::tokenizeRead(rawReadStatement), InvalidSyntaxException);

    rawReadStatement = "read must!BeWrong";
    REQUIRE_THROWS_AS(Tokenizer::tokenizeRead(rawReadStatement), InvalidSyntaxException);

    rawReadStatement = "read 091209NoNumbersAtStart";
    REQUIRE_THROWS_AS(Tokenizer::tokenizeRead(rawReadStatement), InvalidSyntaxException);
}

TEST_CASE("tokenizePrint - Positive Case") {
    string rawPrintStatement;
    shared_ptr<PrintStatement> result;

    rawPrintStatement = "print v";
    result = Tokenizer::tokenizePrint(rawPrintStatement);
    REQUIRE(result->getVariableName() == "v");

    rawPrintStatement = "print variableWithNumbers12345";
    result = Tokenizer::tokenizePrint(rawPrintStatement);
    REQUIRE(result->getVariableName() == "variableWithNumbers12345");

    rawPrintStatement = "print A0123456R";
    result = Tokenizer::tokenizePrint(rawPrintStatement);
    REQUIRE(result->getVariableName() == "A0123456R");

    rawPrintStatement = "print VeryLongVeryLongVeryLongVeryLongVeryLongVeryLongVeryLongVeryLongVeryLongVeryLongVeryLongVeryLong";
    result = Tokenizer::tokenizePrint(rawPrintStatement);
    REQUIRE(result->getVariableName() == "VeryLongVeryLongVeryLongVeryLongVeryLongVeryLongVeryLongVeryLongVeryLongVeryLongVeryLongVeryLong");
}

TEST_CASE("tokenizePrint - Negative Case") {
    string rawPrintStatement;

    rawPrintStatement = "print 999";
    REQUIRE_THROWS_AS(Tokenizer::tokenizePrint(rawPrintStatement), InvalidSyntaxException);

    rawPrintStatement = "print brackets)";
    REQUIRE_THROWS_AS(Tokenizer::tokenizePrint(rawPrintStatement), InvalidSyntaxException);

    rawPrintStatement = "print not(GonnaWork";
    REQUIRE_THROWS_AS(Tokenizer::tokenizePrint(rawPrintStatement), InvalidSyntaxException);

    rawPrintStatement = "print cmon...";
    REQUIRE_THROWS_AS(Tokenizer::tokenizePrint(rawPrintStatement), InvalidSyntaxException);

    rawPrintStatement = "print Invalid?";
    REQUIRE_THROWS_AS(Tokenizer::tokenizePrint(rawPrintStatement), InvalidSyntaxException);

    rawPrintStatement = "print must!BeWrong";
    REQUIRE_THROWS_AS(Tokenizer::tokenizePrint(rawPrintStatement), InvalidSyntaxException);

    rawPrintStatement = "print 091209NoNumbersAtStart";
    REQUIRE_THROWS_AS(Tokenizer::tokenizePrint(rawPrintStatement), InvalidSyntaxException);
}

TEST_CASE("tokenizeAssign - Positive Case") {
    string rawAssignStatement;
    shared_ptr<AssignStatement> result;
    shared_ptr<OperatedExpression> operatedExpression;
    shared_ptr<ConstantExpression> constantExpression;
    shared_ptr<NameExpression> nameExpression;

    rawAssignStatement = "length = radius * 10";
    result = Tokenizer::tokenizeAssign(rawAssignStatement);
    REQUIRE(result->getVarName() == "length");
    operatedExpression = dynamic_pointer_cast<OperatedExpression>(result->getRelFactor());
    REQUIRE(operatedExpression->getOperator() == Operator::MULTIPLY);
    nameExpression = dynamic_pointer_cast<NameExpression>(operatedExpression->getExpression1());
    constantExpression = dynamic_pointer_cast<ConstantExpression>(operatedExpression->getExpression2());
    REQUIRE(nameExpression->getVarName() == "radius");
    REQUIRE(constantExpression->getValue() == 10);
}

TEST_CASE("tokenizeCall - Positive Case") {
    string rawCallStatement;
    shared_ptr<CallStatement> result;

    rawCallStatement = "call v";
    result = Tokenizer::tokenizeCall(rawCallStatement);
    REQUIRE(result->getProcedureName() == "v");

    rawCallStatement = "call variableWithNumbers12345";
    result = Tokenizer::tokenizeCall(rawCallStatement);
    REQUIRE(result->getProcedureName() == "variableWithNumbers12345");

    rawCallStatement = "call A0123456R";
    result = Tokenizer::tokenizeCall(rawCallStatement);
    REQUIRE(result->getProcedureName() == "A0123456R");

    rawCallStatement = "call VeryLongVeryLongVeryLongVeryLongVeryLongVeryLongVeryLongVeryLongVeryLongVeryLongVeryLongVeryLong";
    result = Tokenizer::tokenizeCall(rawCallStatement);
    REQUIRE(result->getProcedureName() == "VeryLongVeryLongVeryLongVeryLongVeryLongVeryLongVeryLongVeryLongVeryLongVeryLongVeryLongVeryLong");
}

TEST_CASE("tokenizeRelFactor - Positive Case - Addition / Subtraction") {
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

TEST_CASE("tokenizeRelFactor - Positive Case - Multiplication / Division / Modulo") {
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

TEST_CASE("tokenizeRelFactor - Positive Case -  ALL Operators") {
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

TEST_CASE("tokenizeRelFactor - Positive Case - Annoying Brackets") {
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

TEST_CASE("tokenizeRelFactor - Positive Case - Brackets reversing the order") {
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

TEST_CASE("tokenizeRelFactor - Positive Case - Brackets changing priority") {
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

TEST_CASE("generateString (relFactor) - Positive Case") {
    string rawRelFactor = "a%b-c/d+e%f*g+h-i";
    shared_ptr<RelationalFactor> relFactor = Tokenizer::tokenizeRelFactor(rawRelFactor);
    REQUIRE(relFactor->generateString() == "(((((a % b) - (c / d)) + ((e % f) * g)) + h) - i)");

    rawRelFactor = "(a + b) * (3 + (x * 3 * 2 % ((1-2) / q)) + 9)";
    relFactor = Tokenizer::tokenizeRelFactor(rawRelFactor);
    REQUIRE(relFactor->generateString() == "((a + b) * ((3 + (((x * 3) * 2) % ((1 - 2) / q))) + 9))");
}
