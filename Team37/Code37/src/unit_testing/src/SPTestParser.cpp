#include <catch.hpp>
#include <string>
#include <vector>

#include "SP/SP.h"
#include "SP/Parser.h"
#include "SP/InvalidSyntaxException.h"
#include "AST/Statement/AssignStatement.h"
#include "AST/Statement/ReadStatement.h"
#include "AST/Statement/CallStatement.h"
#include "AST/Statement/PrintStatement.h"
#include "AST/Statement/IfStatement.h"
#include "AST/Statement/WhileStatement.h"
#include "AST/Expression/RelationalFactor/ConstantExpression.h"
#include "AST/Expression/RelationalFactor/NameExpression.h"
#include "AST/Expression/RelationalFactor/OperatedExpression.h"

using namespace std;

/*
 * Naming Convention:
 * functionName - Positive/Negative Case - (Opt: Others)
 *
 * E.g.
 * extractProcedures - Negative Case - Very Nested Blocks
 */

TEST_CASE("extractProcedures - Positive Case") {
    vector<string> procedures;
    string rawProcedure;
    string sourceCode;
    vector<string> result;

    rawProcedure = "procedure main {\n"
                   "\tprint = x;\n"
                   "\tread x;\n"
                   "\tleft = x;\n"
                   "\tright = left + 2;\n"
                   "\tread temp;\n"
                   "\tradius = x + left;\n"
                   "\ttemp = temp + 2;\n"
                   "\tradius1 = x + left;\n"
                   "}";
    procedures.push_back(rawProcedure);

    rawProcedure = "procedure somethingHmm {\n"
                   "\twhile (iter <=  5) {\n"
                   "\t\tx = right + 3 + 2 + left;\n"
                   "\t\ttemp = temp + 7;\n"
                   "\t}\n"
                   "\tif (iter != 1) then {\n"
                   "\t\titer = iter + 1; \n"
                   "\t\tlength = radius * 10;\n"
                   "\t\tbreadth = radius + 10;\n"
                   "\t} else {\n"
                   "\t\twhile ((left + 1 > 2) || (radius + 1 > 2)) {\n"
                   "\t\t\tbreadth = radius - 100;\n"
                   "\t\t\tlength = left + right;\n"
                   "\t\t}\n"
                   "\t}\n"
                   "}";
    procedures.push_back(rawProcedure);

    rawProcedure = "procedure nestedBlocks {\n"
                   "\tif (iter != 1) then {\n"
                   "\t\titer = iter + 1; \n"
                   "\t\tlength = radius * 10;\n"
                   "\t\tbreadth = radius + 10;\n"
                   "\t} else {\n"
                   "\t\twhile ((left + 1 > 2) || (radius + 1 > 2)) {\n"
                   "\t\t\tbreadth = radius - 100;\n"
                   "\t\t\tlength = left + right;\n"
                   "\t\t\tif (1 > 2) then {\n"
                   "\t\t\t\tprocedure  = 5;\n"
                   "\t\t\t} else {\n"
                   "\t\t\t\twhile = no;\n"
                   "\t\t\t\twhile ((x > 2) || !(y <= 5)) {\n"
                   "\t\t\t\t\tif = while;\n"
                   "\t\t\t\t}\n"
                   "\t\t\t}\n"
                   "\t\t}\n"
                   "\t}\n"
                   "}";
    procedures.push_back(rawProcedure);

    sourceCode = "procedure main {\n"
                 "\tprint = x;\n"
                 "\tread x;\n"
                 "\tleft = x;\n"
                 "\tright = left + 2;\n"
                 "\tread temp;\n"
                 "\tradius = x + left;\n"
                 "\ttemp = temp + 2;\n"
                 "\tradius1 = x + left;\n"
                 "}\n"
                 "\n"
                 "procedure somethingHmm {\n"
                 "\twhile (iter <=  5) {\n"
                 "\t\tx = right + 3 + 2 + left;\n"
                 "\t\ttemp = temp + 7;\n"
                 "\t}\n"
                 "\tif (iter != 1) then {\n"
                 "\t\titer = iter + 1; \n"
                 "\t\tlength = radius * 10;\n"
                 "\t\tbreadth = radius + 10;\n"
                 "\t} else {\n"
                 "\t\twhile ((left + 1 > 2) || (radius + 1 > 2)) {\n"
                 "\t\t\tbreadth = radius - 100;\n"
                 "\t\t\tlength = left + right;\n"
                 "\t\t}\n"
                 "\t}\n"
                 "}\n"
                 "\n"
                 "procedure nestedBlocks {\n"
                 "\tif (iter != 1) then {\n"
                 "\t\titer = iter + 1; \n"
                 "\t\tlength = radius * 10;\n"
                 "\t\tbreadth = radius + 10;\n"
                 "\t} else {\n"
                 "\t\twhile ((left + 1 > 2) || (radius + 1 > 2)) {\n"
                 "\t\t\tbreadth = radius - 100;\n"
                 "\t\t\tlength = left + right;\n"
                 "\t\t\tif (1 > 2) then {\n"
                 "\t\t\t\tprocedure  = 5;\n"
                 "\t\t\t} else {\n"
                 "\t\t\t\twhile = no;\n"
                 "\t\t\t\twhile ((x > 2) || !(y <= 5)) {\n"
                 "\t\t\t\t\tif = while;\n"
                 "\t\t\t\t}\n"
                 "\t\t\t}\n"
                 "\t\t}\n"
                 "\t}\n"
                 "}";

    result = Parser::extractProcedures(sourceCode, result);

    REQUIRE(result == procedures);
}

TEST_CASE("extractProcName - Positive Case") {
    string rawProcedure;
    string result;

    rawProcedure = "procedure main {\n"
                   "\tprint = x;\n"
                   "\tread x;\n"
                   "\tleft = x;\n"
                   "\tright = left + 2;\n"
                   "\tread temp;\n"
                   "\tradius = x + left;\n"
                   "\ttemp = temp + 2;\n"
                   "\tradius1 = x + left;\n"
                   "}";
    result = Parser::extractProcName(rawProcedure);
    REQUIRE(result == "main");

//    rawProcedure = "procedure somethingHmm {\n"
//                   "\twhile (iter <=  5) {\n"
//                   "\t\tx = right + 3 + 2 + left;\n"
//                   "\t\ttemp = temp + 7;\n"
//                   "\t}\n"
//                   "\tif (iter != 1) then {\n"
//                   "\t\titer = iter + 1; \n"
//                   "\t\tlength = radius * 10;\n"
//                   "\t\tbreadth = radius + 10;\n"
//                   "\t} else {\n"
//                   "\t\twhile ((left + 1 > 2) || (radius + 1 > 2)) {\n"
//                   "\t\t\tbreadth = radius - 100;\n"
//                   "\t\t\tlength = left + right;\n"
//                   "\t\t}\n"
//                   "\t}\n"
//                   "}";
//    result = Parser::extractProcName(rawProcedure);
//    REQUIRE(result == "somethingHmm");
//
//    rawProcedure = "procedure nestedBlocks {\n"
//                   "\tif (iter != 1) then {\n"
//                   "\t\titer = iter + 1; \n"
//                   "\t\tlength = radius * 10;\n"
//                   "\t\tbreadth = radius + 10;\n"
//                   "\t} else {\n"
//                   "\t\twhile ((left + 1 > 2) || (radius + 1 > 2)) {\n"
//                   "\t\t\tbreadth = radius - 100;\n"
//                   "\t\t\tlength = left + right;\n"
//                   "\t\t\tif (1 > 2) then {\n"
//                   "\t\t\t\tprocedure  = 5;\n"
//                   "\t\t\t} else {\n"
//                   "\t\t\t\twhile = no;\n"
//                   "\t\t\t\twhile ((x > 2) || !(y <= 5)) {\n"
//                   "\t\t\t\t\tif = while;\n"
//                   "\t\t\t\t}\n"
//                   "\t\t\t}\n"
//                   "\t\t}\n"
//                   "\t}\n"
//                   "}";
//    result = Parser::extractProcName(rawProcedure);
//    REQUIRE(result == "nestedBlocks");
}

TEST_CASE("extractStatements - Positive Case") {
    string rawStatements;
    vector<string> result;

    rawStatements = "print = x;\n"
                    "\tread x;\n"
                    "\tleft = x;\n"
                    "\tright = left + 2;\n"
                    "\tread temp;\n"
                    "\tradius = x + left;\n"
                    "\ttemp = temp + 2;\n"
                    "\tradius1 = x + left;\n"
                    "\twhile (iter <=  5) {\n"
                    "\t\tx = right + 3 + 2 + left;\n"
                    "\t\ttemp = temp + 7;\n"
                    "\t}\n"
                    "\tif (iter != 1) then {\n"
                    "\t\titer = iter + 1; \n"
                    "\t\tlength = radius * 10;\n"
                    "\t\tbreadth = radius + 10;\n"
                    "\t} else {\n"
                    "\t\twhile ((left + 1 > 2) || (radius + 1 > 2)) {\n"
                    "\t\t\tbreadth = radius - 100;\n"
                    "\t\t\tlength = left + right;\n"
                    "\t\t}\n"
                    "\t}\n"
                    "\tif (iter != 1) then {\n"
                    "\t\titer = iter + 1; \n"
                    "\t\tlength = radius * 10;\n"
                    "\t\tbreadth = radius + 10;\n"
                    "\t} else {\n"
                    "\t\twhile ((left + 1 > 2) || (radius + 1 > 2)) {\n"
                    "\t\t\tbreadth = radius - 100;\n"
                    "\t\t\tlength = left + right;\n"
                    "\t\t\tif (1 > 2) then {\n"
                    "\t\t\t\tprocedure  = 5;\n"
                    "\t\t\t} else {\n"
                    "\t\t\t\twhile = no;\n"
                    "\t\t\t\twhile ((x > 2) || !(y <= 5)) {\n"
                    "\t\t\t\t\tif = while;\n"
                    "\t\t\t\t}\n"
                    "\t\t\t}\n"
                    "\t\t}\n"
                    "\t}";

    result = Parser::extractStatements(rawStatements, result);

    REQUIRE(result[0] == "print = x");
    REQUIRE(result[1] == "read x");
    REQUIRE(result[2] == "left = x");
    REQUIRE(result[3] == "right = left + 2");
    REQUIRE(result[4] == "read temp");
    REQUIRE(result[5] == "radius = x + left");
    REQUIRE(result[6] == "temp = temp + 2");
    REQUIRE(result[7] == "radius1 = x + left");
    REQUIRE(result[8] == "while (iter <=  5) {\n"
                         "\t\tx = right + 3 + 2 + left;\n"
                         "\t\ttemp = temp + 7;\n"
                         "\t}");
    REQUIRE(result[9] == "if (iter != 1) then {\n"
                         "\t\titer = iter + 1; \n"
                         "\t\tlength = radius * 10;\n"
                         "\t\tbreadth = radius + 10;\n"
                         "\t} else {\n"
                         "\t\twhile ((left + 1 > 2) || (radius + 1 > 2)) {\n"
                         "\t\t\tbreadth = radius - 100;\n"
                         "\t\t\tlength = left + right;\n"
                         "\t\t}\n"
                         "\t}");
    REQUIRE(result[10] == "if (iter != 1) then {\n"
                         "\t\titer = iter + 1; \n"
                         "\t\tlength = radius * 10;\n"
                         "\t\tbreadth = radius + 10;\n"
                         "\t} else {\n"
                         "\t\twhile ((left + 1 > 2) || (radius + 1 > 2)) {\n"
                         "\t\t\tbreadth = radius - 100;\n"
                         "\t\t\tlength = left + right;\n"
                         "\t\t\tif (1 > 2) then {\n"
                         "\t\t\t\tprocedure  = 5;\n"
                         "\t\t\t} else {\n"
                         "\t\t\t\twhile = no;\n"
                         "\t\t\t\twhile ((x > 2) || !(y <= 5)) {\n"
                         "\t\t\t\t\tif = while;\n"
                         "\t\t\t\t}\n"
                         "\t\t\t}\n"
                         "\t\t}\n"
                         "\t}");
}

TEST_CASE("extractStatements - Negative Case - Missing semicolon") {
    string str = "procedure ABC {\n"
                 "\t\t\t\tread x\n"
                 "\t\t}";
    vector<string> stmtLst;
    REQUIRE_THROWS_AS(Parser::extractStatements(str, stmtLst), InvalidSyntaxException);
}

TEST_CASE("extractConditionalExpr - Positive Case") {
    string rawStatementString;
    string expected;
    string result;

    rawStatementString = "if ((x != 9) && (x <= y)) then {";
    expected = "(x != 9) && (x <= y)";
    result = Parser::extractConditionalExpr(rawStatementString);
    REQUIRE(result == expected);

    rawStatementString = "while((x != 9) && (x <= y))";
    expected = "(x != 9) && (x <= y)";
    result = Parser::extractConditionalExpr(rawStatementString);
    REQUIRE(result == expected);

    rawStatementString = "if(((x != 9)&&(x <= y)))then{";
    expected = "((x != 9)&&(x <= y))";
    result = Parser::extractConditionalExpr(rawStatementString);
    REQUIRE(result == expected);

    rawStatementString = "while (! ((1==0) && (1==0)))";
    expected = "! ((1==0) && (1==0))";
    result = Parser::extractConditionalExpr(rawStatementString);
    REQUIRE(result == expected);
}

TEST_CASE("extractConditionalExpr - Positive Case - Contains RelFactor 1") {
    string stmt = "while (1>= 1%((0-1)) )";
    string expected = "1>= 1%((0-1))";
    string result = Parser::extractConditionalExpr(stmt);
    REQUIRE(result == expected);
}

TEST_CASE("extractConditionalExpr - Positive Case - Contains RelFactor 2") {
    string stmt = "while (1>= 1%((1)) )";
    string expected = "1>= 1%((1))";
    string result = Parser::extractConditionalExpr(stmt);
    REQUIRE(result == expected);
}

TEST_CASE("removeProcedureWrapper - Positive Case") {
    string rawProcedure;
    string expected;
    string result;

    rawProcedure = "procedure somethingHmm {\n"
                   "\twhile (iter <=  5) {\n"
                   "\t\tx = right + 3 + 2 + left;\n"
                   "\t\ttemp = temp + 7;\n"
                   "\t}\n"
                   "\tif (iter != 1) then {\n"
                   "\t\titer = iter + 1; \n"
                   "\t\tlength = radius * 10;\n"
                   "\t\tbreadth = radius + 10;\n"
                   "\t} else {\n"
                   "\t\twhile ((left + 1 > 2) || (radius + 1 > 2)) {\n"
                   "\t\t\tbreadth = radius - 100;\n"
                   "\t\t\tlength = left + right;\n"
                   "\t\t}\n"
                   "\t}\n"
                   "}";
    expected = "while (iter <=  5) {\n"
               "\t\tx = right + 3 + 2 + left;\n"
               "\t\ttemp = temp + 7;\n"
               "\t}\n"
               "\tif (iter != 1) then {\n"
               "\t\titer = iter + 1; \n"
               "\t\tlength = radius * 10;\n"
               "\t\tbreadth = radius + 10;\n"
               "\t} else {\n"
               "\t\twhile ((left + 1 > 2) || (radius + 1 > 2)) {\n"
               "\t\t\tbreadth = radius - 100;\n"
               "\t\t\tlength = left + right;\n"
               "\t\t}\n"
               "\t}";
    result = Parser::removeProcedureWrapper(rawProcedure);
    REQUIRE(result == expected);

    rawProcedure = "procedure main {\n"
                   "\tprint = x;\n"
                   "\tread x;\n"
                   "\tleft = x;\n"
                   "\tright = left + 2;\n"
                   "\tread temp;\n"
                   "\tradius = x + left;\n"
                   "\ttemp = temp + 2;\n"
                   "\tradius1 = x + left;\n"
                   "}";
    expected = "print = x;\n"
               "\tread x;\n"
               "\tleft = x;\n"
               "\tright = left + 2;\n"
               "\tread temp;\n"
               "\tradius = x + left;\n"
               "\ttemp = temp + 2;\n"
               "\tradius1 = x + left;";
    result = Parser::removeProcedureWrapper(rawProcedure);
    REQUIRE(result == expected);

    rawProcedure = "procedure nestedBlocks {\n"
                   "\tif (iter != 1) then {\n"
                   "\t\titer = iter + 1; \n"
                   "\t\tlength = radius * 10;\n"
                   "\t\tbreadth = radius + 10;\n"
                   "\t} else {\n"
                   "\t\twhile ((left + 1 > 2) || (radius + 1 > 2)) {\n"
                   "\t\t\tbreadth = radius - 100;\n"
                   "\t\t\tlength = left + right;\n"
                   "\t\t\tif (1 > 2) then {\n"
                   "\t\t\t\tprocedure  = 5;\n"
                   "\t\t\t} else {\n"
                   "\t\t\t\twhile = no;\n"
                   "\t\t\t\twhile ((x > 2) || !(y <= 5)) {\n"
                   "\t\t\t\t\tif = while;\n"
                   "\t\t\t\t}\n"
                   "\t\t\t}\n"
                   "\t\t}\n"
                   "\t}\n"
                   "}";
    expected = "if (iter != 1) then {\n"
               "\t\titer = iter + 1; \n"
               "\t\tlength = radius * 10;\n"
               "\t\tbreadth = radius + 10;\n"
               "\t} else {\n"
               "\t\twhile ((left + 1 > 2) || (radius + 1 > 2)) {\n"
               "\t\t\tbreadth = radius - 100;\n"
               "\t\t\tlength = left + right;\n"
               "\t\t\tif (1 > 2) then {\n"
               "\t\t\t\tprocedure  = 5;\n"
               "\t\t\t} else {\n"
               "\t\t\t\twhile = no;\n"
               "\t\t\t\twhile ((x > 2) || !(y <= 5)) {\n"
               "\t\t\t\t\tif = while;\n"
               "\t\t\t\t}\n"
               "\t\t\t}\n"
               "\t\t}\n"
               "\t}";
    result = Parser::removeProcedureWrapper(rawProcedure);
    REQUIRE(result == expected);
}

TEST_CASE("parseSourceCode - Positive Case - Statement Number Related") {
    string rawSourceCode;
    shared_ptr<SourceCode> sourceCodeNode;
    Statement::resetLineNumCount();

    rawSourceCode = "procedure main {\n"
                    "\tprint = x;\n"
                    "\tread x;\n"
                    "\tleft = x;\n"
                    "\tright = left + 2;\n"
                    "\tread temp;\n"
                    "\tradius = x + left;\n"
                    "\ttemp = temp + 2;\n"
                    "\tradius1 = x + left;\n"
                    "}\n"
                    "\n"
                    "procedure somethingHmm {\n"
                    "\twhile (((iter <=  5))) {\n"
                    "\t\tx = right + 3 + 2 + left;\n"
                    "\t\ttemp = temp + 7;\n"
                    "\t}\n"
                    "\tif ((((iter != 1)))) then {\n"
                    "\t\titer = iter + 1; \n"
                    "\t\tlength = radius * 10;\n"
                    "\t\tbreadth = radius + 10;\n"
                    "\t} else {\n"
                    "\t\twhile ((((((left)))) + 1 > 2) || ((radius + ((1))) > 2)) {\n"
                    "\t\t\tbreadth = radius - 100;\n"
                    "\t\t\tlength = left + right;\n"
                    "\t\t}\n"
                    "\t}\n"
                    "}\n"
                    "\n"
                    "procedure nestedBlocks {\n"
                    "\tif (iter != 1) then {\n"
                    "\t\titer = iter + 1; \n"
                    "\t\tlength = radius * 10;\n"
                    "\t\tbreadth = radius + 10;\n"
                    "\t} else {\n"
                    "\t\twhile ((left + 1 > 2) || (radius + 1 > 2)) {\n"
                    "\t\t\tbreadth = radius - 100;\n"
                    "\t\t\tlength = left + right;\n"
                    "\t\t\tif (((1 > 2))) then {\n"
                    "\t\t\t\tprocedure  = 5;\n"
                    "\t\t\t} else {\n"
                    "\t\t\t\tlength = no;\n"
                    "\t\t\t\twhile ((x > 2) || !(y <= 5)) {\n"
                    "\t\t\t\t\tbreadth = while;\n"
                    "\t\t\t\t}\n"
                    "\t\t\t}\n"
                    "\t\t}\n"
                    "\t}\n"
                    "}";

    sourceCodeNode = Parser::parseSourceCode(rawSourceCode, "filename");
    REQUIRE(sourceCodeNode->getNumOfStatements() == 30);

    int currStatementNumber = 1;
    vector<shared_ptr<Procedure>> procedures = sourceCodeNode->getProcedures();

    vector<shared_ptr<Statement>> statements = procedures[0]->getStatements();
    for (shared_ptr<Statement> s: statements) {
        REQUIRE(s->getLineNum() == currStatementNumber);
        currStatementNumber++;
    }

    statements = procedures[1]->getStatements();

    REQUIRE(statements[0]->getLineNum() == currStatementNumber);
    currStatementNumber++;

    for (shared_ptr<Statement> s: dynamic_pointer_cast<WhileStatement>(statements[0])->getStatements()) {
        REQUIRE(s->getLineNum() == currStatementNumber);
        currStatementNumber++;
    }

    REQUIRE(statements[1]->getLineNum() == currStatementNumber);
    currStatementNumber++;

    for (shared_ptr<Statement> s: dynamic_pointer_cast<IfStatement>(statements[1])->getThenStatements()) {
        REQUIRE(s->getLineNum() == currStatementNumber);
        currStatementNumber++;
    }

    statements = dynamic_pointer_cast<IfStatement>(statements[1])->getElseStatements();
    REQUIRE(statements[0]->getLineNum() == currStatementNumber);
    currStatementNumber++;

    for (shared_ptr<Statement> s: dynamic_pointer_cast<WhileStatement>(statements[0])->getStatements()) {
        REQUIRE(s->getLineNum() == currStatementNumber);
        currStatementNumber++;
    }

    statements = procedures[2]->getStatements();
    REQUIRE(statements[0]->getLineNum() == currStatementNumber);
    currStatementNumber++;

    for (shared_ptr<Statement> s: dynamic_pointer_cast<IfStatement>(statements[0])->getThenStatements()) {
        REQUIRE(s->getLineNum() == currStatementNumber);
        currStatementNumber++;
    }

    statements =  dynamic_pointer_cast<IfStatement>(statements[0])->getElseStatements();
    REQUIRE(statements[0]->getLineNum() == currStatementNumber);
    currStatementNumber++;

    statements =  dynamic_pointer_cast<WhileStatement>(statements[0])->getStatements();
    REQUIRE(statements[0]->getLineNum() == currStatementNumber);
    currStatementNumber++;
    REQUIRE(statements[1]->getLineNum() == currStatementNumber);
    currStatementNumber++;
    REQUIRE(statements[2]->getLineNum() == currStatementNumber);
    currStatementNumber++;

    for (shared_ptr<Statement> s: dynamic_pointer_cast<IfStatement>(statements[2])->getThenStatements()) {
        REQUIRE(s->getLineNum() == currStatementNumber);
        currStatementNumber++;
    }

    statements = dynamic_pointer_cast<IfStatement>(statements[2])->getElseStatements();
    REQUIRE(statements[0]->getLineNum() == currStatementNumber);
    currStatementNumber++;
    REQUIRE(statements[1]->getLineNum() == currStatementNumber);
    currStatementNumber++;

    statements =  dynamic_pointer_cast<WhileStatement>(statements[1])->getStatements();
    REQUIRE(statements[0]->getLineNum() == currStatementNumber);
}

TEST_CASE("parseSourceCode - Positive Case - EASY 1") {
    string rawSourceCode = "procedure main {\n"
                           "\tprint = x;\n"
                           "\tread x;\n"
                           "\tleft = x;\n"
                           "\tright = left + 2;\n"
                           "\tread temp;\n"
                           "\tradius = x + left;\n"
                           "\ttemp = temp + 2;\n"
                           "\tradius1 = x + left;\n"
                           "}\n"
                           "\n"
                           "procedure somethingHmm {\n"
                           "\twhile (iter <=  5) {\n"
                           "\t\tx = right + 3 + 2 + left;\n"
                           "\t\ttemp = temp + 7;\n"
                           "\t}\n"
                           "\tif (iter != 1) then {\n"
                           "\t\titer = iter + 1; \n"
                           "\t\tlength = radius * 10;\n"
                           "\t\tbreadth = radius + 10;\n"
                           "\t} else {\n"
                           "\t\twhile ((left + 1 > 2) || (radius + 1 > 2)) {\n"
                           "\t\t\tbreadth = radius - 100;\n"
                           "\t\t\tlength = left + right;\n"
                           "\t\t}\n"
                           "\t}\n"
                           "}\n"
                           "\n"
                           "procedure nestedBlocks {\n"
                           "\tif (iter != 1) then {\n"
                           "\t\titer = iter + 1; \n"
                           "\t\tlength = radius * 10;\n"
                           "\t\tbreadth = radius + 10;\n"
                           "\t} else {\n"
                           "\t\twhile ((left + 1 > 2) || (radius + 1 > 2)) {\n"
                           "\t\t\tbreadth = radius - 100;\n"
                           "\t\t\tlength = left + right;\n"
                           "\t\t\tif (1 > 2) then {\n"
                           "\t\t\t\tprocedure  = 5;\n"
                           "\t\t\t} else {\n"
                           "\t\t\t\tlength = no;\n"
                           "\t\t\t\twhile ((x > 2) || !(y <= 5)) {\n"
                           "\t\t\t\t\tbreadth = while;\n"
                           "\t\t\t\t}\n"
                           "\t\t\t}\n"
                           "\t\t}\n"
                           "\t}\n"
                           "}";

    Statement::resetLineNumCount();
    shared_ptr<SourceCode> AST = Parser::parseSourceCode(rawSourceCode, "filename");
    REQUIRE(AST->getNumOfStatements() == 30);
}

TEST_CASE("parseSourceCode - Positive Case - EASY 2") {
    string rawSourceCode = "procedure main {\n"
                           "\tprint = x;\n"
                           "\tread x;\n"
                           "\tleft = x;\n"
                           "\tcall somethingHmm;\n"
                           "\tright = left + 2;\n"
                           "\tread temp;\n"
                           "\tradius = x + left;\n"
                           "\ttemp = temp + 2;\n"
                           "\tradius1 = x + left;\n"
                           "\tcall anotherOne;\n"
                           "}\n"
                           "\n"
                           "procedure somethingHmm {\n"
                           "\twhile (iter <=  5) {\n"
                           "\t\tx = right + 3 + 2 + left;\n"
                           "\t\ttemp = temp + 7;\n"
                           "\t\tcall anotherOne;\n"
                           "\t}\n"
                           "\tx = 5;\n"
                           "\tif (iter != 1) then {\n"
                           "\t\titer = iter + 1; \n"
                           "\t\tlength = radius * 10;\n"
                           "\t\tbreadth = radius + 10;\n"
                           "\t} else {\n"
                           "\t\twhile ((left + 1 > 2) || (radius + 1 > 2)) {\n"
                           "\t\t\tcall nestedBlocks;\n"
                           "\t\t\tlength = left + right;\n"
                           "\t\t}\n"
                           "\t}\n"
                           "}\n"
                           "\n"
                           "procedure nestedBlocks {\n"
                           "\tif (iter != 1) then {\n"
                           "\t\titer = iter + 1; \n"
                           "\t\tlength = radius * 10;\n"
                           "\t\tbreadth = radius + 10;\n"
                           "\t} else {\n"
                           "\t\twhile ((left + 1 > 2) || (radius + 1 > 2)) {\n"
                           "\t\t\tbreadth = radius - 100;\n"
                           "\t\t\tlength = left + right;\n"
                           "\t\t\tcall anotherOne;\n"
                           "\t\t\tif (1 > 2) then {\n"
                           "\t\t\t\tprocedure  = 5;\n"
                           "\t\t\t} else {\n"
                           "\t\t\t\tlength = no;\n"
                           "\t\t\t\twhile ((x > 2) || !(y <= 5)) {\n"
                           "\t\t\t\t\tcall anotherOne;\n"
                           "\t\t\t\t}\n"
                           "\t\t\t}\n"
                           "\t\t}\n"
                           "\t}\n"
                           "}\n"
                           "\n"
                           "procedure anotherOne {\n"
                           "\tprint x;\n"
                           "}";

    Statement::resetLineNumCount();
    shared_ptr<SourceCode> AST = Parser::parseSourceCode(rawSourceCode, "filename");
    REQUIRE(AST->getNumOfStatements() == 36);
}

TEST_CASE("parseSourceCode - Positive Case - HARD 1") {
    string rawSourceCode = "procedure procedure {\n"
                           "  while (while < while) {\n"
                           "    if (if != if * (2 + 1)) then {\n"
                           "      then = then + if;\n"
                           "      while = procedure + 32432 + if + then;\n"
                           "      else = while;\n"
                           "    } else {\n"
                           "      else = if + then + procedure;\n"
                           "      while (5 > 0) {\n"
                           "        if (then > (else - procedure) * (then / 9342) + while) then {\n"
                           "          print print;\n"
                           "        } else {\n"
                           "          while (procedure >= then) {\n"
                           "            print call;\n"
                           "          }\n"
                           "        }\n"
                           "      }\n"
                           "      while (procedure <= else + while) {\n"
                           "        if ((x + 1) * (2 / 3) == 1 + 2 / 6 % 3 + 5 + 6 * 2 % 2) then {\n"
                           "          if (read == print) then {\n"
                           "            read x;\n"
                           "          } else {\n"
                           "            print x;\n"
                           "          }\n"
                           "          while (x < y) {\n"
                           "            call doNothing;\n"
                           "          }\n"
                           "        } else {\n"
                           "          while (else < if) {\n"
                           "            call doNothing;\n"
                           "          }\n"
                           "        }\n"
                           "      }\n"
                           "    }\n"
                           "  }\n"
                           "}\n"
                           "\n"
                           "procedure doNothing {\n"
                           "  while = 1 + doNothing % doSomething * ifThen - print + a9345934un34ubhr / 23423453;\n"
                           "  ifThen = 2;\n"
                           "  if = 3;\n"
                           "  then = 4;\n"
                           "  else = 5;\n"
                           "}\n"
                           "\n"
                           "procedure doSomething {\n"
                           "  call doNothing;\n"
                           "  read doNothing;\n"
                           "  read while;\n"
                           "  print while;\n"
                           "  read ifThen;\n"
                           "  print ifThen;\n"
                           "  read if;\n"
                           "  print if;\n"
                           "  read then;\n"
                           "  print then;\n"
                           "  read else;\n"
                           "  print else;\n"
                           "}";

    shared_ptr<SourceCode> AST;
    shared_ptr<Procedure> procedure;
    shared_ptr<WhileStatement> whileStatement;
    shared_ptr<IfStatement> ifStatement;
    shared_ptr<AssignStatement> assignStatement;
    shared_ptr<CallStatement> callStatement;
    shared_ptr<PrintStatement> printStatement;
    shared_ptr<ReadStatement> readStatement;
    shared_ptr<RelationalExpression> relationalExpression;
    shared_ptr<OperatedExpression> operatedExpression;
    shared_ptr<NameExpression> nameExpression;
    shared_ptr<ConstantExpression> constantExpression;


    Statement::resetLineNumCount();
    AST = Parser::parseSourceCode(rawSourceCode, "filename");
    REQUIRE(AST->getNumOfStatements() == 37);

    procedure = AST->getProcedures()[0];
    REQUIRE(procedure->getProcedureName() == "procedure");
    whileStatement = dynamic_pointer_cast<WhileStatement>(procedure->getStatements()[0]);
    relationalExpression = dynamic_pointer_cast<RelationalExpression>(whileStatement->getConditionalExpression());
    REQUIRE(relationalExpression->getOperator() == RelationalOperator::LESS_THAN);
    nameExpression = dynamic_pointer_cast<NameExpression>(relationalExpression->getRelFactor1());
    REQUIRE(nameExpression->getVarName() == "while");
    nameExpression = dynamic_pointer_cast<NameExpression>(relationalExpression->getRelFactor2());
    REQUIRE(nameExpression->getVarName() == "while");

    ifStatement = dynamic_pointer_cast<IfStatement>(whileStatement->getStatements()[0]);
    relationalExpression = dynamic_pointer_cast<RelationalExpression>(ifStatement->getConditionalExpression());
    REQUIRE(relationalExpression->getOperator() == RelationalOperator::NOT_EQUALS);
    nameExpression = dynamic_pointer_cast<NameExpression>(relationalExpression->getRelFactor1());
    REQUIRE(nameExpression->getVarName() == "if");
    operatedExpression = dynamic_pointer_cast<OperatedExpression>(relationalExpression->getRelFactor2());
    REQUIRE(operatedExpression->generateString() == "((if) * ((2) + (1)))");

    assignStatement = dynamic_pointer_cast<AssignStatement>(ifStatement->getThenStatements()[0]);
    REQUIRE(assignStatement->getVarName() == "then");
    REQUIRE(assignStatement->getRelFactor()->generateString() == "((then) + (if))");

    assignStatement = dynamic_pointer_cast<AssignStatement>(ifStatement->getThenStatements()[1]);
    REQUIRE(assignStatement->getVarName() == "while");
    REQUIRE(assignStatement->getRelFactor()->generateString() == "((((procedure) + (32432)) + (if)) + (then))");

    assignStatement = dynamic_pointer_cast<AssignStatement>(ifStatement->getThenStatements()[2]);
    REQUIRE(assignStatement->getVarName() == "else");
    REQUIRE(assignStatement->getRelFactor()->generateString() == "(while)");

    assignStatement = dynamic_pointer_cast<AssignStatement>(ifStatement->getElseStatements()[0]);
    REQUIRE(assignStatement->getVarName() == "else");
    REQUIRE(assignStatement->getRelFactor()->generateString() == "(((if) + (then)) + (procedure))");

    shared_ptr<IfStatement> ifStatementSavePoint;

    ifStatementSavePoint = ifStatement;

    whileStatement = dynamic_pointer_cast<WhileStatement>(ifStatement->getElseStatements()[1]);
    relationalExpression = dynamic_pointer_cast<RelationalExpression>(whileStatement->getConditionalExpression());
    REQUIRE(relationalExpression->getOperator() == RelationalOperator::GREATER_THAN);
    constantExpression = dynamic_pointer_cast<ConstantExpression>(relationalExpression->getRelFactor1());
    REQUIRE(constantExpression->getValue() == 5);
    constantExpression = dynamic_pointer_cast<ConstantExpression>(relationalExpression->getRelFactor2());
    REQUIRE(constantExpression->getValue() == 0);

    ifStatement = dynamic_pointer_cast<IfStatement>(whileStatement->getStatements()[0]);
    relationalExpression = dynamic_pointer_cast<RelationalExpression>(ifStatement->getConditionalExpression());
    REQUIRE(relationalExpression->getOperator() == RelationalOperator::GREATER_THAN);
    nameExpression = dynamic_pointer_cast<NameExpression>(relationalExpression->getRelFactor1());
    REQUIRE(nameExpression->getVarName() == "then");
    operatedExpression = dynamic_pointer_cast<OperatedExpression>(relationalExpression->getRelFactor2());
    REQUIRE(operatedExpression->generateString() == "((((else) - (procedure)) * ((then) / (9342))) + (while))");

    printStatement = dynamic_pointer_cast<PrintStatement>(ifStatement->getThenStatements()[0]);
    REQUIRE(printStatement->getVariableName() == "print");

    whileStatement = dynamic_pointer_cast<WhileStatement>(ifStatement->getElseStatements()[0]);
    relationalExpression = dynamic_pointer_cast<RelationalExpression>(whileStatement->getConditionalExpression());
    REQUIRE(relationalExpression->getOperator() == RelationalOperator::GREATER_THAN_OR_EQUALS);
    nameExpression = dynamic_pointer_cast<NameExpression>(relationalExpression->getRelFactor1());
    REQUIRE(nameExpression->getVarName() == "procedure");
    nameExpression = dynamic_pointer_cast<NameExpression>(relationalExpression->getRelFactor2());
    REQUIRE(nameExpression->getVarName() == "then");

    whileStatement = dynamic_pointer_cast<WhileStatement>(ifStatementSavePoint->getElseStatements()[2]);
    relationalExpression = dynamic_pointer_cast<RelationalExpression>(whileStatement->getConditionalExpression());
    REQUIRE(relationalExpression->getOperator() == RelationalOperator::LESS_THAN_OR_EQUALS);
    nameExpression = dynamic_pointer_cast<NameExpression>(relationalExpression->getRelFactor1());
    REQUIRE(nameExpression->getVarName() == "procedure");
    operatedExpression = dynamic_pointer_cast<OperatedExpression>(relationalExpression->getRelFactor2());
    REQUIRE(operatedExpression->generateString() == "((else) + (while))");

    ifStatement = dynamic_pointer_cast<IfStatement>(whileStatement->getStatements()[0]);
    relationalExpression = dynamic_pointer_cast<RelationalExpression>(ifStatement->getConditionalExpression());
    REQUIRE(relationalExpression->getOperator() == RelationalOperator::EQUALS);
    operatedExpression = dynamic_pointer_cast<OperatedExpression>(relationalExpression->getRelFactor1());
    REQUIRE(operatedExpression->generateString() == "(((x) + (1)) * ((2) / (3)))");
    operatedExpression = dynamic_pointer_cast<OperatedExpression>(relationalExpression->getRelFactor2());
    REQUIRE(operatedExpression->generateString() == "((((1) + (((2) / (6)) % (3))) + (5)) + (((6) * (2)) % (2)))");

    ifStatementSavePoint = ifStatement;

    ifStatement = dynamic_pointer_cast<IfStatement>(ifStatement->getThenStatements()[0]);
    relationalExpression = dynamic_pointer_cast<RelationalExpression>(ifStatement->getConditionalExpression());
    REQUIRE(relationalExpression->getOperator() == RelationalOperator::EQUALS);
    nameExpression = dynamic_pointer_cast<NameExpression>(relationalExpression->getRelFactor1());
    REQUIRE(nameExpression->getVarName() == "read");
    nameExpression = dynamic_pointer_cast<NameExpression>(relationalExpression->getRelFactor2());
    REQUIRE(nameExpression->getVarName() == "print");

    readStatement = dynamic_pointer_cast<ReadStatement>(ifStatement->getThenStatements()[0]);
    REQUIRE(readStatement->getVariableName() == "x");
    printStatement = dynamic_pointer_cast<PrintStatement>(ifStatement->getElseStatements()[0]);
    REQUIRE(printStatement->getVariableName() == "x");

    whileStatement = dynamic_pointer_cast<WhileStatement>(ifStatementSavePoint->getThenStatements()[1]);
    relationalExpression = dynamic_pointer_cast<RelationalExpression>(whileStatement->getConditionalExpression());
    REQUIRE(relationalExpression->getOperator() == RelationalOperator::LESS_THAN);
    nameExpression = dynamic_pointer_cast<NameExpression>(relationalExpression->getRelFactor1());
    REQUIRE(nameExpression->getVarName() == "x");
    nameExpression = dynamic_pointer_cast<NameExpression>(relationalExpression->getRelFactor2());
    REQUIRE(nameExpression->getVarName() == "y");

    callStatement = dynamic_pointer_cast<CallStatement>(whileStatement->getStatements()[0]);
    REQUIRE(callStatement->getProcedureName() == "doNothing");

    whileStatement = dynamic_pointer_cast<WhileStatement>(ifStatementSavePoint->getElseStatements()[0]);
    relationalExpression = dynamic_pointer_cast<RelationalExpression>(whileStatement->getConditionalExpression());
    REQUIRE(relationalExpression->getOperator() == RelationalOperator::LESS_THAN);
    nameExpression = dynamic_pointer_cast<NameExpression>(relationalExpression->getRelFactor1());
    REQUIRE(nameExpression->getVarName() == "else");
    nameExpression = dynamic_pointer_cast<NameExpression>(relationalExpression->getRelFactor2());
    REQUIRE(nameExpression->getVarName() == "if");

    callStatement = dynamic_pointer_cast<CallStatement>(whileStatement->getStatements()[0]);
    REQUIRE(callStatement->getProcedureName() == "doNothing");


    procedure = AST->getProcedures()[1];
    REQUIRE(procedure->getProcedureName() == "doNothing");
    assignStatement = dynamic_pointer_cast<AssignStatement>(procedure->getStatements()[0]);
    REQUIRE(assignStatement->getVarName() == "while");
    REQUIRE(assignStatement->getRelFactor()->generateString() ==
            "((((1) + (((doNothing) % (doSomething)) * (ifThen))) - (print)) + ((a9345934un34ubhr) / (23423453)))");
    assignStatement = dynamic_pointer_cast<AssignStatement>(procedure->getStatements()[1]);
    REQUIRE(assignStatement->getVarName() == "ifThen");
    REQUIRE(assignStatement->getRelFactor()->generateString() == "(2)");
    assignStatement = dynamic_pointer_cast<AssignStatement>(procedure->getStatements()[2]);
    REQUIRE(assignStatement->getVarName() == "if");
    REQUIRE(assignStatement->getRelFactor()->generateString() == "(3)");
    assignStatement = dynamic_pointer_cast<AssignStatement>(procedure->getStatements()[3]);
    REQUIRE(assignStatement->getVarName() == "then");
    REQUIRE(assignStatement->getRelFactor()->generateString() == "(4)");
    assignStatement = dynamic_pointer_cast<AssignStatement>(procedure->getStatements()[4]);
    REQUIRE(assignStatement->getVarName() == "else");
    REQUIRE(assignStatement->getRelFactor()->generateString() == "(5)");


    procedure = AST->getProcedures()[2];
    callStatement = dynamic_pointer_cast<CallStatement>(procedure->getStatements()[0]);
    REQUIRE(callStatement->getProcedureName() == "doNothing");
    readStatement = dynamic_pointer_cast<ReadStatement>(procedure->getStatements()[1]);
    REQUIRE(readStatement->getVariableName() == "doNothing");
    readStatement = dynamic_pointer_cast<ReadStatement>(procedure->getStatements()[2]);
    REQUIRE(readStatement->getVariableName() == "while");
    printStatement = dynamic_pointer_cast<PrintStatement>(procedure->getStatements()[3]);
    REQUIRE(printStatement->getVariableName() == "while");
    readStatement = dynamic_pointer_cast<ReadStatement>(procedure->getStatements()[4]);
    REQUIRE(readStatement->getVariableName() == "ifThen");
    printStatement = dynamic_pointer_cast<PrintStatement>(procedure->getStatements()[5]);
    REQUIRE(printStatement->getVariableName() == "ifThen");
    readStatement = dynamic_pointer_cast<ReadStatement>(procedure->getStatements()[6]);
    REQUIRE(readStatement->getVariableName() == "if");
    printStatement = dynamic_pointer_cast<PrintStatement>(procedure->getStatements()[7]);
    REQUIRE(printStatement->getVariableName() == "if");
    readStatement = dynamic_pointer_cast<ReadStatement>(procedure->getStatements()[8]);
    REQUIRE(readStatement->getVariableName() == "then");
    printStatement = dynamic_pointer_cast<PrintStatement>(procedure->getStatements()[9]);
    REQUIRE(printStatement->getVariableName() == "then");
    readStatement = dynamic_pointer_cast<ReadStatement>(procedure->getStatements()[10]);
    REQUIRE(readStatement->getVariableName() == "else");
    printStatement = dynamic_pointer_cast<PrintStatement>(procedure->getStatements()[11]);
    REQUIRE(printStatement->getVariableName() == "else");
}

TEST_CASE("parseSourceCode - Positive Case - HARD 2") {
    string rawSourceCode = "procedure procedure1 {\n"
                           "  while (while < while) { if (if != if * (2 + 1)) then { then = then + if; while = procedure + 32432 + if + then;\n"
                           "      else = while; } else { else = if + then + procedure; while (5 > 0) { if (then > (else - procedure) * (then / 9342) + while) then { print print;\n"
                           "        } else { while (procedure >= then) { call procedure4; }}} while (procedure <= else + while) {\n"
                           "        if ((x + 1) * (2 / 3) == 1 + 2 / 6 % 3 + 5 + 6 * 2 % 2) then { if (read == print) then { call procedure3; } else { print x; }\n"
                           "        while (x < y) { call procedure2; }} else { while (else < if) { call procedure2; }}}}}\n"
                           "}\n"
                           "\n"
                           "procedure procedure2 {\n"
                           "  while (while < while) { if (if != if * (2 + 1)) then { then = then + if; while = procedure + 32432 + if + then;\n"
                           "      else = while; } else { else = if + then + procedure; while (5 > 0) { if (then > (else - procedure) * (then / 9342) + while) then { print print;\n"
                           "        } else { while (procedure >= then) { call procedure4; }}} while (procedure <= else + while) {\n"
                           "        if ((x + 1) * (2 / 3) == 1 + 2 / 6 % 3 + 5 + 6 * 2 % 2) then { if (read == print) then { read x; } else { print x; }\n"
                           "        while (x < y) { call procedure3; }} else { while (else < if) { call procedure3; }}}}}\n"
                           "}\n"
                           "\n"
                           "procedure procedure3 {\n"
                           "  while (while < while) { if (if != if * (2 + 1)) then { then = then + if; while = procedure + 32432 + if + then;\n"
                           "      else = while; } else { else = if + then + procedure; while (5 > 0) { if (then > (else - procedure) * (then / 9342) + while) then { print print;\n"
                           "        } else { while (procedure >= then) { print call; }}} while (procedure <= else + while) {\n"
                           "        if ((x + 1) * (2 / 3) == 1 + 2 / 6 % 3 + 5 + 6 * 2 % 2) then { if (read == print) then { read x; } else { print x; }\n"
                           "        while (x < y) { call procedure4; }} else { while (else < if) { call procedure4; }}}}}\n"
                           "}\n"
                           "\n"
                           "procedure procedure4 {\n"
                           "  while (while < while) { if (if != if * (2 + 1)) then { then = then + if; while = procedure + 32432 + if + then;\n"
                           "      else = while; } else { else = if + then + procedure; while (5 > 0) { if (then > (else - procedure) * (then / 9342) + while) then { print print;\n"
                           "        } else { while (procedure >= then) { print call; }}} while (procedure <= else + while) {\n"
                           "        if ((x + 1) * (2 / 3) == 1 + 2 / 6 % 3 + 5 + 6 * 2 % 2) then { if (read == print) then { read x; } else { print x; }\n"
                           "        while (x < y) { call procedure5; }} else { while (else < if) { call procedure5; }}}}}\n"
                           "}\n"
                           "\n"
                           "procedure procedure5 {\n"
                           "  while (while < while) { if (if != if * (2 + 1)) then { call procedure12; call procedure15;\n"
                           "      else = while; } else { else = if + then + procedure; while (5 > 0) { if (then > (else - procedure) * (then / 9342) + while) then { print print;\n"
                           "        } else { while (procedure >= then) { call end; }}} while (procedure <= else + while) {\n"
                           "        if ((x + 1) * (2 / 3) == 1 + 2 / 6 % 3 + 5 + 6 * 2 % 2) then { if (read == print) then { call end; } else { print x; }\n"
                           "        while (x < y) { call procedure6; }} else { while (else < if) { call procedure6; }}}}}\n"
                           "}\n"
                           "\n"
                           "procedure procedure6 {\n"
                           "  while (while < while) { if (if != if * (2 + 1)) then { call procedure16; call procedure12;\n"
                           "      else = while; } else { else = if + then + procedure; while (5 > 0) { if (then > (else - procedure) * (then / 9342) + while) then { print print;\n"
                           "        } else { while (procedure >= then) { call end; }}} while (procedure <= else + while) {\n"
                           "        if ((x + 1) * (2 / 3) == 1 + 2 / 6 % 3 + 5 + 6 * 2 % 2) then { if (read == print) then { call end; } else { print x; }\n"
                           "        while (x < y) { call procedure7; }} else { while (else < if) { call procedure7; }}}}}\n"
                           "}\n"
                           "\n"
                           "procedure procedure7 {\n"
                           "  while (while < while) { if (if != if * (2 + 1)) then { call end; call procedure13;\n"
                           "      else = while; } else { else = if + then + procedure; while (5 > 0) { if (then > (else - procedure) * (then / 9342) + while) then { print print;\n"
                           "        } else { while (procedure >= then) { call end; }}} while (procedure <= else + while) {\n"
                           "        if ((x + 1) * (2 / 3) == 1 + 2 / 6 % 3 + 5 + 6 * 2 % 2) then { if (read == print) then { call procedure25; } else { print x; }\n"
                           "        while (x < y) { call procedure8; }} else { while (else < if) { call procedure8; }}}}}\n"
                           "}\n"
                           "\n"
                           "procedure procedure8 {\n"
                           "  while (while < while) { if (if != if * (2 + 1)) then { call procedure45; call procedure37;\n"
                           "      else = while; } else { else = if + then + procedure; while (5 > 0) { if (then > (else - procedure) * (then / 9342) + while) then { print print;\n"
                           "        } else { while (procedure >= then) { call end; }}} while (procedure <= else + while) {\n"
                           "        if ((x + 1) * (2 / 3) == 1 + 2 / 6 % 3 + 5 + 6 * 2 % 2) then { if (read == print) then { call end; } else { print x; }\n"
                           "        while (x < y) { call procedure9; }} else { while (else < if) { call procedure9; }}}}}\n"
                           "}\n"
                           "\n"
                           "procedure procedure9 {\n"
                           "  while (while < while) { if (if != if * (2 + 1)) then { call procedure13; call end;\n"
                           "      else = while; } else { else = if + then + procedure; while (5 > 0) { if (then > (else - procedure) * (then / 9342) + while) then { print print;\n"
                           "        } else { while (procedure >= then) { call end; }}} while (procedure <= else + while) {\n"
                           "        if ((x + 1) * (2 / 3) == 1 + 2 / 6 % 3 + 5 + 6 * 2 % 2) then { if (read == print) then { call procedure12; } else { print x; }\n"
                           "        while (x < y) { call procedure10; }} else { while (else < if) { call procedure10; }}}}}\n"
                           "}\n"
                           "\n"
                           "procedure procedure10 {\n"
                           "  while (while < while) { if (if != if * (2 + 1)) then { call end; call end;\n"
                           "      else = while; } else { else = if + then + procedure; while (5 > 0) { if (then > (else - procedure) * (then / 9342) + while) then { print print;\n"
                           "        } else { while (procedure >= then) { call end; }}} while (procedure <= else + while) {\n"
                           "        if ((x + 1) * (2 / 3) == 1 + 2 / 6 % 3 + 5 + 6 * 2 % 2) then { if (read == print) then { call procedure48; } else { print x; }\n"
                           "        while (x < y) { call procedure11; }} else { while (else < if) { call procedure11; }}}}}\n"
                           "}\n"
                           "\n"
                           "procedure procedure11 {\n"
                           "  while (while < while) { if (if != if * (2 + 1)) then { call procedure25; call end;\n"
                           "      else = while; } else { else = if + then + procedure; while (5 > 0) { if (then > (else - procedure) * (then / 9342) + while) then { print print;\n"
                           "        } else { while (procedure >= then) { call end; }}} while (procedure <= else + while) {\n"
                           "        if ((x + 1) * (2 / 3) == 1 + 2 / 6 % 3 + 5 + 6 * 2 % 2) then { if (read == print) then { call procedure37; } else { print x; }\n"
                           "        while (x < y) { call procedure12; }} else { while (else < if) { call procedure12; }}}}}\n"
                           "}\n"
                           "\n"
                           "procedure procedure12 {\n"
                           "  while (while < while) { if (if != if * (2 + 1)) then { call procedure19; call procedure15;\n"
                           "      else = while; } else { else = if + then + procedure; while (5 > 0) { if (then > (else - procedure) * (then / 9342) + while) then { print print;\n"
                           "        } else { while (procedure >= then) { call end; }}} while (procedure <= else + while) {\n"
                           "        if ((x + 1) * (2 / 3) == 1 + 2 / 6 % 3 + 5 + 6 * 2 % 2) then { if (read == print) then { call end; } else { print x; }\n"
                           "        while (x < y) { call procedure13; }} else { while (else < if) { call procedure13; }}}}}\n"
                           "}\n"
                           "\n"
                           "procedure procedure13 {\n"
                           "  while (while < while) { if (if != if * (2 + 1)) then { call end; call procedure20;\n"
                           "      else = while; } else { else = if + then + procedure; while (5 > 0) { if (then > (else - procedure) * (then / 9342) + while) then { print print;\n"
                           "        } else { while (procedure >= then) { call end; }}} while (procedure <= else + while) {\n"
                           "        if ((x + 1) * (2 / 3) == 1 + 2 / 6 % 3 + 5 + 6 * 2 % 2) then { if (read == print) then { call end; } else { print x; }\n"
                           "        while (x < y) { call procedure14; }} else { while (else < if) { call procedure14; }}}}}\n"
                           "}\n"
                           "\n"
                           "procedure procedure14 {\n"
                           "  while (while < while) { if (if != if * (2 + 1)) then { call procedure44; call procedure47;\n"
                           "      else = while; } else { else = if + then + procedure; while (5 > 0) { if (then > (else - procedure) * (then / 9342) + while) then { print print;\n"
                           "        } else { while (procedure >= then) { call end; }}} while (procedure <= else + while) {\n"
                           "        if ((x + 1) * (2 / 3) == 1 + 2 / 6 % 3 + 5 + 6 * 2 % 2) then { if (read == print) then { call procedure37; } else { print x; }\n"
                           "        while (x < y) { call procedure15; }} else { while (else < if) { call procedure15; }}}}}\n"
                           "}\n"
                           "\n"
                           "procedure procedure15 {\n"
                           "  while (while < while) { if (if != if * (2 + 1)) then { call end; call procedure45;\n"
                           "      else = while; } else { else = if + then + procedure; while (5 > 0) { if (then > (else - procedure) * (then / 9342) + while) then { print print;\n"
                           "        } else { while (procedure >= then) { call end; }}} while (procedure <= else + while) {\n"
                           "        if ((x + 1) * (2 / 3) == 1 + 2 / 6 % 3 + 5 + 6 * 2 % 2) then { if (read == print) then { call procedure25; } else { print x; }\n"
                           "        while (x < y) { call procedure16; }} else { while (else < if) { call procedure16; }}}}}\n"
                           "}\n"
                           "\n"
                           "procedure procedure16 {\n"
                           "  while (while < while) { if (if != if * (2 + 1)) then { call procedure20; call procedure47;\n"
                           "      else = while; } else { else = if + then + procedure; while (5 > 0) { if (then > (else - procedure) * (then / 9342) + while) then { print print;\n"
                           "        } else { while (procedure >= then) { call end; }}} while (procedure <= else + while) {\n"
                           "        if ((x + 1) * (2 / 3) == 1 + 2 / 6 % 3 + 5 + 6 * 2 % 2) then { if (read == print) then { call procedure48; } else { print x; }\n"
                           "        while (x < y) { call procedure17; }} else { while (else < if) { call procedure17; }}}}}\n"
                           "}\n"
                           "\n"
                           "procedure procedure17 {\n"
                           "  while (while < while) { if (if != if * (2 + 1)) then { call end; call end;\n"
                           "      else = while; } else { else = if + then + procedure; while (5 > 0) { if (then > (else - procedure) * (then / 9342) + while) then { print print;\n"
                           "        } else { while (procedure >= then) { call end; }}} while (procedure <= else + while) {\n"
                           "        if ((x + 1) * (2 / 3) == 1 + 2 / 6 % 3 + 5 + 6 * 2 % 2) then { if (read == print) then { call procedure41; } else { print x; }\n"
                           "        while (x < y) { call procedure18; }} else { while (else < if) { call procedure18; }}}}}\n"
                           "}\n"
                           "\n"
                           "procedure procedure18 {\n"
                           "  while (while < while) { if (if != if * (2 + 1)) then { call procedure21; call procedure20;\n"
                           "      else = while; } else { else = if + then + procedure; while (5 > 0) { if (then > (else - procedure) * (then / 9342) + while) then { print print;\n"
                           "        } else { while (procedure >= then) { call end; }}} while (procedure <= else + while) {\n"
                           "        if ((x + 1) * (2 / 3) == 1 + 2 / 6 % 3 + 5 + 6 * 2 % 2) then { if (read == print) then { call end; } else { print x; }\n"
                           "        while (x < y) { call procedure19; }} else { while (else < if) { call procedure19; }}}}}\n"
                           "}\n"
                           "\n"
                           "procedure procedure19 {\n"
                           "  while (while < while) { if (if != if * (2 + 1)) then { call procedure21; call procedure24;\n"
                           "      else = while; } else { else = if + then + procedure; while (5 > 0) { if (then > (else - procedure) * (then / 9342) + while) then { print print;\n"
                           "        } else { while (procedure >= then) { call end; }}} while (procedure <= else + while) {\n"
                           "        if ((x + 1) * (2 / 3) == 1 + 2 / 6 % 3 + 5 + 6 * 2 % 2) then { if (read == print) then { call procedure37; } else { print x; }\n"
                           "        while (x < y) { call procedure20; }} else { while (else < if) { call procedure20; }}}}}\n"
                           "}\n"
                           "\n"
                           "procedure procedure20 {\n"
                           "  while (while < while) { if (if != if * (2 + 1)) then { call procedure45; call procedure44;\n"
                           "      else = while; } else { else = if + then + procedure; while (5 > 0) { if (then > (else - procedure) * (then / 9342) + while) then { print print;\n"
                           "        } else { while (procedure >= then) { call end; }}} while (procedure <= else + while) {\n"
                           "        if ((x + 1) * (2 / 3) == 1 + 2 / 6 % 3 + 5 + 6 * 2 % 2) then { if (read == print) then { call end; } else { print x; }\n"
                           "        while (x < y) { call procedure21; }} else { while (else < if) { call procedure21; }}}}}\n"
                           "}\n"
                           "\n"
                           "procedure procedure21 {\n"
                           "  while (while < while) { if (if != if * (2 + 1)) then { call end; call procedure33;\n"
                           "      else = while; } else { else = if + then + procedure; while (5 > 0) { if (then > (else - procedure) * (then / 9342) + while) then { print print;\n"
                           "        } else { while (procedure >= then) { call end; }}} while (procedure <= else + while) {\n"
                           "        if ((x + 1) * (2 / 3) == 1 + 2 / 6 % 3 + 5 + 6 * 2 % 2) then { if (read == print) then { call procedure25; } else { print x; }\n"
                           "        while (x < y) { call procedure22; }} else { while (else < if) { call procedure22; }}}}}\n"
                           "}\n"
                           "\n"
                           "procedure procedure22 {\n"
                           "  while (while < while) { if (if != if * (2 + 1)) then { call end; call procedure45;\n"
                           "      else = while; } else { else = if + then + procedure; while (5 > 0) { if (then > (else - procedure) * (then / 9342) + while) then { print print;\n"
                           "        } else { while (procedure >= then) { call end; }}} while (procedure <= else + while) {\n"
                           "        if ((x + 1) * (2 / 3) == 1 + 2 / 6 % 3 + 5 + 6 * 2 % 2) then { if (read == print) then { call end; } else { print x; }\n"
                           "        while (x < y) { call procedure23; }} else { while (else < if) { call procedure23; }}}}}\n"
                           "}\n"
                           "\n"
                           "procedure procedure23 {\n"
                           "  while (while < while) { if (if != if * (2 + 1)) then { call end; call procedure35;\n"
                           "      else = while; } else { else = if + then + procedure; while (5 > 0) { if (then > (else - procedure) * (then / 9342) + while) then { print print;\n"
                           "        } else { while (procedure >= then) { call end; }}} while (procedure <= else + while) {\n"
                           "        if ((x + 1) * (2 / 3) == 1 + 2 / 6 % 3 + 5 + 6 * 2 % 2) then { if (read == print) then { call procedure48; } else { print x; }\n"
                           "        while (x < y) { call procedure24; }} else { while (else < if) { call procedure24; }}}}}\n"
                           "}\n"
                           "\n"
                           "procedure procedure24 {\n"
                           "  while (while < while) { if (if != if * (2 + 1)) then { call procedure44; call end;\n"
                           "      else = while; } else { else = if + then + procedure; while (5 > 0) { if (then > (else - procedure) * (then / 9342) + while) then { print print;\n"
                           "        } else { while (procedure >= then) { call end; }}} while (procedure <= else + while) {\n"
                           "        if ((x + 1) * (2 / 3) == 1 + 2 / 6 % 3 + 5 + 6 * 2 % 2) then { if (read == print) then { call procedure37; } else { print x; }\n"
                           "        while (x < y) { call procedure25; }} else { while (else < if) { call procedure25; }}}}}\n"
                           "}\n"
                           "\n"
                           "procedure procedure25 {\n"
                           "  while (while < while) { if (if != if * (2 + 1)) then { call procedure35; call end;\n"
                           "      else = while; } else { else = if + then + procedure; while (5 > 0) { if (then > (else - procedure) * (then / 9342) + while) then { print print;\n"
                           "        } else { while (procedure >= then) { call end; }}} while (procedure <= else + while) {\n"
                           "        if ((x + 1) * (2 / 3) == 1 + 2 / 6 % 3 + 5 + 6 * 2 % 2) then { if (read == print) then { call end; } else { print x; }\n"
                           "        while (x < y) { call procedure26; }} else { while (else < if) { call procedure26; }}}}}\n"
                           "}\n"
                           "\n"
                           "procedure procedure26 {\n"
                           "  while (while < while) { if (if != if * (2 + 1)) then { call procedure33; call end;\n"
                           "      else = while; } else { else = if + then + procedure; while (5 > 0) { if (then > (else - procedure) * (then / 9342) + while) then { print print;\n"
                           "        } else { while (procedure >= then) { call end; }}} while (procedure <= else + while) {\n"
                           "        if ((x + 1) * (2 / 3) == 1 + 2 / 6 % 3 + 5 + 6 * 2 % 2) then { if (read == print) then { call procedure41; } else { print x; }\n"
                           "        while (x < y) { call procedure27; }} else { while (else < if) { call procedure27; }}}}}\n"
                           "}\n"
                           "\n"
                           "procedure procedure27 {\n"
                           "  while (while < while) { if (if != if * (2 + 1)) then { call end; call procedure44;\n"
                           "      else = while; } else { else = if + then + procedure; while (5 > 0) { if (then > (else - procedure) * (then / 9342) + while) then { print print;\n"
                           "        } else { while (procedure >= then) { call end; }}} while (procedure <= else + while) {\n"
                           "        if ((x + 1) * (2 / 3) == 1 + 2 / 6 % 3 + 5 + 6 * 2 % 2) then { if (read == print) then { call procedure35; } else { print x; }\n"
                           "        while (x < y) { call procedure28; }} else { while (else < if) { call procedure28; }}}}}\n"
                           "}\n"
                           "\n"
                           "procedure procedure28 {\n"
                           "  while (while < while) { if (if != if * (2 + 1)) then { call end; call procedure33;\n"
                           "      else = while; } else { else = if + then + procedure; while (5 > 0) { if (then > (else - procedure) * (then / 9342) + while) then { print print;\n"
                           "        } else { while (procedure >= then) { call end; }}} while (procedure <= else + while) {\n"
                           "        if ((x + 1) * (2 / 3) == 1 + 2 / 6 % 3 + 5 + 6 * 2 % 2) then { if (read == print) then { call end; } else { print x; }\n"
                           "        while (x < y) { call procedure29; }} else { while (else < if) { call procedure29; }}}}}\n"
                           "}\n"
                           "\n"
                           "procedure procedure29 {\n"
                           "  while (while < while) { if (if != if * (2 + 1)) then { call end; call end;\n"
                           "      else = while; } else { else = if + then + procedure; while (5 > 0) { if (then > (else - procedure) * (then / 9342) + while) then { print print;\n"
                           "        } else { while (procedure >= then) { call end; }}} while (procedure <= else + while) {\n"
                           "        if ((x + 1) * (2 / 3) == 1 + 2 / 6 % 3 + 5 + 6 * 2 % 2) then { if (read == print) then { call procedure48; } else { print x; }\n"
                           "        while (x < y) { call procedure30; }} else { while (else < if) { call procedure30; }}}}}\n"
                           "}\n"
                           "\n"
                           "procedure procedure30 {\n"
                           "  while (while < while) { if (if != if * (2 + 1)) then { call end; call end;\n"
                           "      else = while; } else { else = if + then + procedure; while (5 > 0) { if (then > (else - procedure) * (then / 9342) + while) then { print print;\n"
                           "        } else { while (procedure >= then) { call end; }}} while (procedure <= else + while) {\n"
                           "        if ((x + 1) * (2 / 3) == 1 + 2 / 6 % 3 + 5 + 6 * 2 % 2) then { if (read == print) then { call procedure33; } else { print x; }\n"
                           "        while (x < y) { call procedure31; }} else { while (else < if) { call procedure31; }}}}}\n"
                           "}\n"
                           "\n"
                           "procedure procedure31 {\n"
                           "  while (while < while) { if (if != if * (2 + 1)) then { call end; call end;\n"
                           "      else = while; } else { else = if + then + procedure; while (5 > 0) { if (then > (else - procedure) * (then / 9342) + while) then { print print;\n"
                           "        } else { while (procedure >= then) { call end; }}} while (procedure <= else + while) {\n"
                           "        if ((x + 1) * (2 / 3) == 1 + 2 / 6 % 3 + 5 + 6 * 2 % 2) then { if (read == print) then { call procedure35; } else { print x; }\n"
                           "        while (x < y) { call procedure32; }} else { while (else < if) { call procedure32; }}}}}\n"
                           "}\n"
                           "\n"
                           "procedure procedure32 {\n"
                           "  while (while < while) { if (if != if * (2 + 1)) then { call end; call end;\n"
                           "      else = while; } else { else = if + then + procedure; while (5 > 0) { if (then > (else - procedure) * (then / 9342) + while) then { print print;\n"
                           "        } else { while (procedure >= then) { call end; }}} while (procedure <= else + while) {\n"
                           "        if ((x + 1) * (2 / 3) == 1 + 2 / 6 % 3 + 5 + 6 * 2 % 2) then { if (read == print) then { call procedure41; } else { print x; }\n"
                           "        while (x < y) { call procedure33; }} else { while (else < if) { call procedure33; }}}}}\n"
                           "}\n"
                           "\n"
                           "procedure procedure33 {\n"
                           "  while (while < while) { if (if != if * (2 + 1)) then { call procedure35; call procedure48;\n"
                           "      else = while; } else { else = if + then + procedure; while (5 > 0) { if (then > (else - procedure) * (then / 9342) + while) then { print print;\n"
                           "        } else { while (procedure >= then) { call end; }}} while (procedure <= else + while) {\n"
                           "        if ((x + 1) * (2 / 3) == 1 + 2 / 6 % 3 + 5 + 6 * 2 % 2) then { if (read == print) then { call procedure46; } else { print x; }\n"
                           "        while (x < y) { call procedure34; }} else { while (else < if) { call procedure34; }}}}}\n"
                           "}\n"
                           "\n"
                           "procedure procedure34 {\n"
                           "  while (while < while) { if (if != if * (2 + 1)) then { call end; call end;\n"
                           "      else = while; } else { else = if + then + procedure; while (5 > 0) { if (then > (else - procedure) * (then / 9342) + while) then { print print;\n"
                           "        } else { while (procedure >= then) { call end; }}} while (procedure <= else + while) {\n"
                           "        if ((x + 1) * (2 / 3) == 1 + 2 / 6 % 3 + 5 + 6 * 2 % 2) then { if (read == print) then { call end; } else { print x; }\n"
                           "        while (x < y) { call procedure35; }} else { while (else < if) { call procedure35; }}}}}\n"
                           "}\n"
                           "\n"
                           "procedure procedure35 {\n"
                           "  while (while < while) { if (if != if * (2 + 1)) then { call procedure41; call end;\n"
                           "      else = while; } else { else = if + then + procedure; while (5 > 0) { if (then > (else - procedure) * (then / 9342) + while) then { print print;\n"
                           "        } else { while (procedure >= then) { call end; }}} while (procedure <= else + while) {\n"
                           "        if ((x + 1) * (2 / 3) == 1 + 2 / 6 % 3 + 5 + 6 * 2 % 2) then { if (read == print) then { call end; } else { print x; }\n"
                           "        while (x < y) { call procedure36; }} else { while (else < if) { call procedure36; }}}}}\n"
                           "}\n"
                           "\n"
                           "procedure procedure36 {\n"
                           "  while (while < while) { if (if != if * (2 + 1)) then { call procedure42; call end;\n"
                           "      else = while; } else { else = if + then + procedure; while (5 > 0) { if (then > (else - procedure) * (then / 9342) + while) then { print print;\n"
                           "        } else { while (procedure >= then) { call end; }}} while (procedure <= else + while) {\n"
                           "        if ((x + 1) * (2 / 3) == 1 + 2 / 6 % 3 + 5 + 6 * 2 % 2) then { if (read == print) then { call procedure43; } else { print x; }\n"
                           "        while (x < y) { call procedure37; }} else { while (else < if) { call procedure37; }}}}}\n"
                           "}\n"
                           "\n"
                           "procedure procedure37 {\n"
                           "  while (while < while) { if (if != if * (2 + 1)) then { call end; call end;\n"
                           "      else = while; } else { else = if + then + procedure; while (5 > 0) { if (then > (else - procedure) * (then / 9342) + while) then { print print;\n"
                           "        } else { while (procedure >= then) { call end; }}} while (procedure <= else + while) {\n"
                           "        if ((x + 1) * (2 / 3) == 1 + 2 / 6 % 3 + 5 + 6 * 2 % 2) then { if (read == print) then { call end; } else { print x; }\n"
                           "        while (x < y) { call procedure38; }} else { while (else < if) { call procedure38; }}}}}\n"
                           "}\n"
                           "\n"
                           "procedure procedure38 {\n"
                           "  while (while < while) { if (if != if * (2 + 1)) then { call end; call procedure43;\n"
                           "      else = while; } else { else = if + then + procedure; while (5 > 0) { if (then > (else - procedure) * (then / 9342) + while) then { print print;\n"
                           "        } else { while (procedure >= then) { call end; }}} while (procedure <= else + while) {\n"
                           "        if ((x + 1) * (2 / 3) == 1 + 2 / 6 % 3 + 5 + 6 * 2 % 2) then { if (read == print) then { call procedure42; } else { print x; }\n"
                           "        while (x < y) { call procedure39; }} else { while (else < if) { call procedure39; }}}}}\n"
                           "}\n"
                           "\n"
                           "procedure procedure39 {\n"
                           "  while (while < while) { if (if != if * (2 + 1)) then { call procedure46; call procedure41;\n"
                           "      else = while; } else { else = if + then + procedure; while (5 > 0) { if (then > (else - procedure) * (then / 9342) + while) then { print print;\n"
                           "        } else { while (procedure >= then) { call end; }}} while (procedure <= else + while) {\n"
                           "        if ((x + 1) * (2 / 3) == 1 + 2 / 6 % 3 + 5 + 6 * 2 % 2) then { if (read == print) then { call end; } else { print x; }\n"
                           "        while (x < y) { call procedure40; }} else { while (else < if) { call procedure40; }}}}}\n"
                           "}\n"
                           "\n"
                           "procedure procedure40 {\n"
                           "  while (while < while) { if (if != if * (2 + 1)) then { call end; call procedure42;\n"
                           "      else = while; } else { else = if + then + procedure; while (5 > 0) { if (then > (else - procedure) * (then / 9342) + while) then { print print;\n"
                           "        } else { while (procedure >= then) { call end; }}} while (procedure <= else + while) {\n"
                           "        if ((x + 1) * (2 / 3) == 1 + 2 / 6 % 3 + 5 + 6 * 2 % 2) then { if (read == print) then { call end; } else { print x; }\n"
                           "        while (x < y) { call procedure41; }} else { while (else < if) { call procedure41; }}}}}\n"
                           "}\n"
                           "\n"
                           "procedure procedure41 {\n"
                           "  while (while < while) { if (if != if * (2 + 1)) then { call end; call procedure43;\n"
                           "      else = while; } else { else = if + then + procedure; while (5 > 0) { if (then > (else - procedure) * (then / 9342) + while) then { print print;\n"
                           "        } else { while (procedure >= then) { call end; }}} while (procedure <= else + while) {\n"
                           "        if ((x + 1) * (2 / 3) == 1 + 2 / 6 % 3 + 5 + 6 * 2 % 2) then { if (read == print) then { call end; } else { print x; }\n"
                           "        while (x < y) { call procedure42; }} else { while (else < if) { call procedure42; }}}}}\n"
                           "}\n"
                           "\n"
                           "procedure procedure42 {\n"
                           "  while (while < while) { if (if != if * (2 + 1)) then { call procedure46; call end;\n"
                           "      else = while; } else { else = if + then + procedure; while (5 > 0) { if (then > (else - procedure) * (then / 9342) + while) then { print print;\n"
                           "        } else { while (procedure >= then) { call end; }}} while (procedure <= else + while) {\n"
                           "        if ((x + 1) * (2 / 3) == 1 + 2 / 6 % 3 + 5 + 6 * 2 % 2) then { if (read == print) then { call end; } else { print x; }\n"
                           "        while (x < y) { call procedure43; }} else { while (else < if) { call procedure43; }}}}}\n"
                           "}\n"
                           "\n"
                           "procedure procedure43 {\n"
                           "  while (while < while) { if (if != if * (2 + 1)) then { call procedure47; call end;\n"
                           "      else = while; } else { else = if + then + procedure; while (5 > 0) { if (then > (else - procedure) * (then / 9342) + while) then { print print;\n"
                           "        } else { while (procedure >= then) { call end; }}} while (procedure <= else + while) {\n"
                           "        if ((x + 1) * (2 / 3) == 1 + 2 / 6 % 3 + 5 + 6 * 2 % 2) then { if (read == print) then { call end; } else { print x; }\n"
                           "        while (x < y) { call procedure44; }} else { while (else < if) { call procedure44; }}}}}\n"
                           "}\n"
                           "\n"
                           "procedure procedure44 {\n"
                           "  while (while < while) { if (if != if * (2 + 1)) then { call end; call procedure46;\n"
                           "      else = while; } else { else = if + then + procedure; while (5 > 0) { if (then > (else - procedure) * (then / 9342) + while) then { print print;\n"
                           "        } else { while (procedure >= then) { call end; }}} while (procedure <= else + while) {\n"
                           "        if ((x + 1) * (2 / 3) == 1 + 2 / 6 % 3 + 5 + 6 * 2 % 2) then { if (read == print) then { call procedure47; } else { print x; }\n"
                           "        while (x < y) { call procedure45; }} else { while (else < if) { call procedure45; }}}}}\n"
                           "}\n"
                           "\n"
                           "procedure procedure45 {\n"
                           "  while (while < while) { if (if != if * (2 + 1)) then { call end; call end;\n"
                           "      else = while; } else { else = if + then + procedure; while (5 > 0) { if (then > (else - procedure) * (then / 9342) + while) then { print print;\n"
                           "        } else { while (procedure >= then) { call end; }}} while (procedure <= else + while) {\n"
                           "        if ((x + 1) * (2 / 3) == 1 + 2 / 6 % 3 + 5 + 6 * 2 % 2) then { if (read == print) then { call end; } else { print x; }\n"
                           "        while (x < y) { call procedure46; }} else { while (else < if) { call procedure46; }}}}}\n"
                           "}\n"
                           "\n"
                           "procedure procedure46 {\n"
                           "  while (while < while) { if (if != if * (2 + 1)) then { call end; call end;\n"
                           "      else = while; } else { else = if + then + procedure; while (5 > 0) { if (then > (else - procedure) * (then / 9342) + while) then { print print;\n"
                           "        } else { while (procedure >= then) { call end; }}} while (procedure <= else + while) {\n"
                           "        if ((x + 1) * (2 / 3) == 1 + 2 / 6 % 3 + 5 + 6 * 2 % 2) then { if (read == print) then { call end; } else { print x; }\n"
                           "        while (x < y) { call procedure47; }} else { while (else < if) { call procedure47; }}}}}\n"
                           "}\n"
                           "\n"
                           "procedure procedure47 {\n"
                           "  while (while < while) { if (if != if * (2 + 1)) then { call end; call end;\n"
                           "      else = while; } else { else = if + then + procedure; while (5 > 0) { if (then > (else - procedure) * (then / 9342) + while) then { print print;\n"
                           "        } else { while (procedure >= then) { call end; }}} while (procedure <= else + while) {\n"
                           "        if ((x + 1) * (2 / 3) == 1 + 2 / 6 % 3 + 5 + 6 * 2 % 2) then { if (read == print) then { call procedure50; } else { print x; }\n"
                           "        while (x < y) { call procedure48; }} else { while (else < if) { call procedure48; }}}}}\n"
                           "}\n"
                           "\n"
                           "procedure procedure48 {\n"
                           "  while (while < while) { if (if != if * (2 + 1)) then { call procedure50; call end;\n"
                           "      else = while; } else { else = if + then + procedure; while (5 > 0) { if (then > (else - procedure) * (then / 9342) + while) then { print print;\n"
                           "        } else { while (procedure >= then) { call end; }}} while (procedure <= else + while) {\n"
                           "        if ((x + 1) * (2 / 3) == 1 + 2 / 6 % 3 + 5 + 6 * 2 % 2) then { if (read == print) then { call end; } else { print x; }\n"
                           "        while (x < y) { call procedure49; }} else { while (else < if) { call procedure49; }}}}}\n"
                           "}\n"
                           "\n"
                           "procedure procedure49 {\n"
                           "  while (while < while) { if (if != if * (2 + 1)) then { call end; call end;\n"
                           "      else = while; } else { else = if + then + procedure; while (5 > 0) { if (then > (else - procedure) * (then / 9342) + while) then { print print;\n"
                           "        } else { while (procedure >= then) { call end; }}} while (procedure <= else + while) {\n"
                           "        if ((x + 1) * (2 / 3) == 1 + 2 / 6 % 3 + 5 + 6 * 2 % 2) then { if (read == print) then { call end; } else { print x; }\n"
                           "        while (x < y) { call procedure50; }} else { while (else < if) { call procedure50; }}}}}\n"
                           "}\n"
                           "\n"
                           "procedure procedure50 {\n"
                           "  while (while < while) { if (if != if * (2 + 1)) then { call end; call end;\n"
                           "      else = while; } else { else = if + then + procedure; while (5 > 0) { if (then > (else - procedure) * (then / 9342) + while) then { print print;\n"
                           "        } else { while (procedure >= then) { call end; }}} while (procedure <= else + while) {\n"
                           "        if ((x + 1) * (2 / 3) == 1 + 2 / 6 % 3 + 5 + 6 * 2 % 2) then { if (read == print) then { call end; } else { print x; }\n"
                           "        while (x < y) { call end; }} else { while (else < if) { call end; }}}}}\n"
                           "}\n"
                           "\n"
                           "procedure end {\n"
                           "  print x;\n"
                           "}";

    Statement::resetLineNumCount();
    shared_ptr<SourceCode> AST = Parser::parseSourceCode(rawSourceCode, "filename");
    REQUIRE(AST->getNumOfStatements() == 1001);
}

TEST_CASE("parseSourceCode - Negative Case - Extra Semicolons") {
    string rawSourceCode;

    rawSourceCode = "procedure affecting {\n"
                    "  a = a + b + c;\n"
                    "  b = f + g;\n"
                    "  c = a + 5;\n"
                    "  d = a + d;\n"
                    "  e = c - 9;\n"
                    "  f = e + b + c;\n"
                    "  g = a;\n"
                    "  h = d + e;\n"
                    "  while (qwe >= another) {\n"
                    "    A = A + 123;\n"
                    "    B = B + 543;\n"
                    "    B = B - 234;\n"
                    "    C = A + 2;\n"
                    "    D = B + A;\n"
                    "  }\n"
                    "};";

    REQUIRE_THROWS_AS(Parser::parseSourceCode(rawSourceCode, "filename"), InvalidSyntaxException);

    rawSourceCode = "procedure factorial {\n"
                    "  read arg1;\n"
                    "  x = arg1;\n"
                    "  result = 1;\n"
                    "  while (x > 1) {\n"
                    "    result = result * x;\n"
                    "    x = x - 1;;\n"
                    "  }\n"
                    "  print result;\n"
                    "}";
    REQUIRE_THROWS_AS(Parser::parseSourceCode(rawSourceCode, "filename"), InvalidSyntaxException);

    rawSourceCode = ";procedure factorial {\n"
                    "  read arg1;\n"
                    "  x = arg1;\n"
                    "  result = 1;\n"
                    "  while (x > 1) {\n"
                    "    result = result * x;\n"
                    "    x = x - 1;\n"
                    "  }\n"
                    "  print result;\n"
                    "}";
    REQUIRE_THROWS_AS(Parser::parseSourceCode(rawSourceCode, "filename"), InvalidSyntaxException);
}

TEST_CASE("parseSourceCode - Negative Case - Wrong Curly Brackets") {
    string rawSourceCode;

    rawSourceCode = "procedure affecting {\n"
                    "  a = a + b + c;\n"
                    "  b = f + g;\n"
                    "  c = a + 5;\n"
                    "  d = a + d;\n"
                    "  e = c - 9;\n"
                    "  f = e + b + c;\n"
                    "  g = a;\n"
                    "  h = d + e;\n"
                    "  while (qwe >= another) {\n"
                    "    A = A + 123;\n"
                    "    B = B + 543;\n"
                    "    B = B - 234;\n"
                    "    C = A + 2;\n"
                    "    D = B + A;\n"
                    "  }\n"
                    "}}";

    REQUIRE_THROWS_AS(Parser::parseSourceCode(rawSourceCode, "filename"), InvalidSyntaxException);

    rawSourceCode = "procedure factorial {\n"
                    "  read arg1;\n"
                    "  x = arg1;\n"
                    "  result = 1;\n"
                    "  while (x > 1) {\n"
                    "    result = result * x;\n"
                    "    x = x - 1;\n"
                    "  }}\n"
                    "  print result;\n"
                    "}";
    REQUIRE_THROWS_AS(Parser::parseSourceCode(rawSourceCode, "filename"), InvalidSyntaxException);

    rawSourceCode = "procedure factorial {\n"
                    "  read arg1;\n"
                    "  x = arg1;\n"
                    "  result = 1;\n"
                    "  while (x > 1) {{\n"
                    "    result = result * x;\n"
                    "    x = x - 1;\n"
                    "  }\n"
                    "  print result;\n"
                    "}";
    REQUIRE_THROWS_AS(Parser::parseSourceCode(rawSourceCode, "filename"), InvalidSyntaxException);

    rawSourceCode = "procedure factorial {{\n"
                    "  read arg1;\n"
                    "  x = arg1;\n"
                    "  result = 1;\n"
                    "  while (x > 1) {\n"
                    "    result = result * x;\n"
                    "    x = x - 1;\n"
                    "  }\n"
                    "  print result;\n"
                    "}";
    REQUIRE_THROWS_AS(Parser::parseSourceCode(rawSourceCode, "filename"), InvalidSyntaxException);
}

TEST_CASE("parseSourceCode - Negative Case - Wrong Round Brackets") {
    string rawSourceCode;

    rawSourceCode = "procedure factorial {\n"
                    "  read arg1;\n"
                    "  x = arg1;\n"
                    "  result = 1;\n"
                    "  while ((x > 1) {\n"
                    "    result = result * x;\n"
                    "    x = x - 1;\n"
                    "  }\n"
                    "  print result;\n"
                    "}";
    REQUIRE_THROWS_AS(Parser::parseSourceCode(rawSourceCode, "filename"), InvalidSyntaxException);

    rawSourceCode = "procedure factorial {\n"
                    "  read arg1;\n"
                    "  x = arg1;\n"
                    "  result = 1;\n"
                    "  while (x > 1)) {\n"
                    "    result = result * x;\n"
                    "    x = x - 1;\n"
                    "  }\n"
                    "  print result;\n"
                    "}";
    REQUIRE_THROWS_AS(Parser::parseSourceCode(rawSourceCode, "filename"), InvalidSyntaxException);

    rawSourceCode = "procedure factorial {\n"
                    "  read arg1;\n"
                    "  x = arg1;\n"
                    "  result = 1;\n"
                    "  while (x > 1) {\n"
                    "    (result) = result * x;\n"
                    "    x = x - 1;\n"
                    "  }\n"
                    "  print result;\n"
                    "}";
    REQUIRE_THROWS_AS(Parser::parseSourceCode(rawSourceCode, "filename"), InvalidSyntaxException);

    rawSourceCode = "procedure factorial {\n"
                    "  read arg1;\n"
                    "  x = arg1;\n"
                    "  result = 1;\n"
                    "  while x > 1 {\n"
                    "    result = result * x;\n"
                    "    x = x - 1;\n"
                    "  }\n"
                    "  print result;\n"
                    "}";
    REQUIRE_THROWS_AS(Parser::parseSourceCode(rawSourceCode, "filename"), InvalidSyntaxException);
}

TEST_CASE("parseSourceCode - Negative Case - Empty blocks") {
    string rawSourceCode;

    // No procedures
    rawSourceCode = "";
    REQUIRE_THROWS_AS(Parser::parseSourceCode(rawSourceCode, "filename"), InvalidSyntaxException);

    // No statements in procedure
    rawSourceCode = "procedure main {}";
    REQUIRE_THROWS_AS(Parser::parseSourceCode(rawSourceCode, "filename"), InvalidSyntaxException);

    // No statements in if-then block
    rawSourceCode = "procedure main {\n"
                    "\tif (iter != 1) then {\n"
                    "\t} else {\n"
                    "\t\tbreadth = radius - 100;\n"
                    "\t}\n"
                    "}";
    REQUIRE_THROWS_AS(Parser::parseSourceCode(rawSourceCode, "filename"), InvalidSyntaxException);

    // No statements in if-else block
    rawSourceCode = "procedure main {\n"
                    "\tif (iter != 1) then {\n"
                    "\t\tbreadth = radius - 100;\n"
                    "\t} else {\n"
                    "\t}\n"
                    "}";
    REQUIRE_THROWS_AS(Parser::parseSourceCode(rawSourceCode, "filename"), InvalidSyntaxException);

    // No statements in while block
    rawSourceCode = "procedure somethingHmm {\n"
                    "\twhile (iter <=  5) {\n"
                    "\t}\n"
                    "}";
    REQUIRE_THROWS_AS(Parser::parseSourceCode(rawSourceCode, "filename"), InvalidSyntaxException);
}

TEST_CASE("parseProcedure - Positive Case") {
    string rawProcedure;
    shared_ptr<Procedure> procedure;
    vector<shared_ptr<Statement>> statements;

    rawProcedure = "procedure main {\n"
                   "\tprint = x;\n"
                   "\tread x;\n"
                   "\tleft = right;\n"
                   "\tright = left + 2;\n"
                   "}";
    procedure = Parser::parseProcedure(rawProcedure);

    statements = procedure->getStatements();


    REQUIRE(dynamic_pointer_cast<AssignStatement>(statements[0])->getVarName() == "print");
    REQUIRE(dynamic_pointer_cast<NameExpression>(
            dynamic_pointer_cast<AssignStatement>(statements[0])->getRelFactor())->getVarName() == "x");

    REQUIRE(dynamic_pointer_cast<ReadStatement>(statements[1])->getVariableName() == "x");

    REQUIRE(dynamic_pointer_cast<AssignStatement>(statements[2])->getVarName() == "left");
    REQUIRE(dynamic_pointer_cast<NameExpression>(
            dynamic_pointer_cast<AssignStatement>(statements[2])->getRelFactor())->getVarName() == "right");

    REQUIRE(dynamic_pointer_cast<AssignStatement>(statements[3])->getVarName() == "right");
    REQUIRE(dynamic_pointer_cast<OperatedExpression>(
            dynamic_pointer_cast<AssignStatement>(statements[3])->getRelFactor())->getOperator() == Operator::ADD);
    REQUIRE(dynamic_pointer_cast<NameExpression>(
            dynamic_pointer_cast<OperatedExpression>(
                    dynamic_pointer_cast<AssignStatement>(statements[3])->getRelFactor())->getExpression1())->getVarName() == "left");
    REQUIRE(dynamic_pointer_cast<ConstantExpression>(
            dynamic_pointer_cast<OperatedExpression>(
                    dynamic_pointer_cast<AssignStatement>(statements[3])->getRelFactor())->getExpression2())->getValue() == 2);
}

TEST_CASE("parseProcedure - Positive Case - Nested If-Else, bad styling") {
    string str = "procedure Main {\n"
                 "if (x   > 3) then {\n"
                 "if (x > 3 )then {\n"
                 "x=  1;\n"
                 "}  \n"
                 "else{\n"
                 "x=1;\n"
                 "}\n"
                 "}\n"
                 "else   {\n"
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

TEST_CASE("parseStatement - Positive Case") {
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

TEST_CASE("parseIfElse - Positive Case") {
    string rawIfStatement = "if (!(x == y)) then {\n"
                   "\t\t\t\tread x;\n"
                   "\t\t} else {\n"
                   "\t\t\t\tprint x;\n"
                   "\t\t}";

    shared_ptr<IfStatement> ifStatement =
            dynamic_pointer_cast<IfStatement>(Parser::parseStatement(rawIfStatement));
    shared_ptr<NotCondition> condExpr =
            dynamic_pointer_cast<NotCondition>(ifStatement->getConditionalExpression());
    shared_ptr<RelationalExpression> relExpr =
            dynamic_pointer_cast<RelationalExpression>(condExpr->getConditionalExpression());

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

TEST_CASE("parseIfElse - Positive Case - Nested If statement") {
    string str = "if (x > 3) then {\n"
                 "  if (x > 3 )then {\n"
                 "      x=1;\n"
                 "  } else {\n"
                 "      x=1;\n"
                 "  }\n"
                 "} else {\n"
                 "  x=1;\n"
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

TEST_CASE("parseIfElse - Positive Case - Keywords used a variable names") {
    string rawIfStatement;
    shared_ptr<IfStatement> ifStatement;
    shared_ptr<RelationalExpression> relationalExpression;
    shared_ptr<NameExpression> nameExpression;
    shared_ptr<OperatedExpression> operatedExpression;
    shared_ptr<PrintStatement> printStatement;

    rawIfStatement = "if (then > else + if) then {\n"
                     "          print if;\n"
                     "          print then;\n"
                     "          print else;\n"
                     "        } else {\n"
                     "          print if;\n"
                     "          print then;\n"
                     "          print else;\n"
                     "        }";

    ifStatement = Parser::parseIfElse(rawIfStatement);
    relationalExpression = dynamic_pointer_cast<RelationalExpression>(ifStatement->getConditionalExpression());
    REQUIRE(relationalExpression->getOperator() == RelationalOperator::GREATER_THAN);
    nameExpression = dynamic_pointer_cast<NameExpression>(relationalExpression->getRelFactor1());
    REQUIRE(nameExpression->getVarName() == "then");
    operatedExpression = dynamic_pointer_cast<OperatedExpression>(relationalExpression->getRelFactor2());
    REQUIRE(operatedExpression->getOperator() == Operator::ADD);
    nameExpression = dynamic_pointer_cast<NameExpression>(operatedExpression->getExpression1());
    REQUIRE(nameExpression->getVarName() == "else");
    nameExpression = dynamic_pointer_cast<NameExpression>(operatedExpression->getExpression2());
    REQUIRE(nameExpression->getVarName() == "if");

    printStatement = dynamic_pointer_cast<PrintStatement>(ifStatement->getThenStatements()[0]);
    REQUIRE(printStatement->getVariableName() == "if");
    printStatement = dynamic_pointer_cast<PrintStatement>(ifStatement->getThenStatements()[1]);
    REQUIRE(printStatement->getVariableName() == "then");
    printStatement = dynamic_pointer_cast<PrintStatement>(ifStatement->getThenStatements()[2]);
    REQUIRE(printStatement->getVariableName() == "else");

    printStatement = dynamic_pointer_cast<PrintStatement>(ifStatement->getElseStatements()[0]);
    REQUIRE(printStatement->getVariableName() == "if");
    printStatement = dynamic_pointer_cast<PrintStatement>(ifStatement->getElseStatements()[1]);
    REQUIRE(printStatement->getVariableName() == "then");
    printStatement = dynamic_pointer_cast<PrintStatement>(ifStatement->getElseStatements()[2]);
    REQUIRE(printStatement->getVariableName() == "else");
}

TEST_CASE("parseIfElse - Negative Case - if-else missing else block") {
    string str = "if (!(x == y)) then {\n"
                 "\t\t\t\tread x;\n"
                 "\t\t}";
    REQUIRE_THROWS_AS(Parser::parseIfElse(str), InvalidSyntaxException);
}

TEST_CASE("parseIfElse - Negative Case - if-else missing 'then' keyword") {
    string str = "if (iter != 1) {\n"
                 "13\t\titer = iter + 1; \n"
                 "14\t\tlength = radius * 10;\n"
                 "15\t\tbreadth = radius + 10;\n"
                 "\t\t} else {\n"
                 "16\t\twhile ((left + 1 > 2) || (radius + 1 > 2)) {\n"
                 "17\t\t\tbreadth = radius - 100;\n"
                 "18\t\t\tlength = left + right;\n"
                 "\t\t\t}\n"
                 "\t\t}";
    REQUIRE_THROWS_AS(Parser::parseIfElse(str), InvalidSyntaxException);
}

TEST_CASE("parseIfElse - Negative Case - Missing '{' in if block") {
    string str = "if (x>y) then \n"
                 "\t\t\t\tread x;}\n";
    REQUIRE_THROWS_AS(Parser::parseIfElse(str), InvalidSyntaxException);
}

TEST_CASE("parseWhile - Positive Case - Bad styling") {
    string rawWhileStatement = "while    (x >= y)       {\n"
                               "\t\t  print    longlonglonglonglongNAME;\n"
                               "\t  \t       read namewithNUMBERS123053;\n"
                               "\t  \t}   ";

    shared_ptr<RelationalExpression> relExpr;
    shared_ptr<NameExpression> nameExpr;
    shared_ptr<PrintStatement> printStmt;
    shared_ptr<ReadStatement> readStmt;

    shared_ptr<WhileStatement> whileStatement = dynamic_pointer_cast<WhileStatement>(Parser::parseStatement(rawWhileStatement));
    relExpr = dynamic_pointer_cast<RelationalExpression>(whileStatement->getConditionalExpression());
    REQUIRE(relExpr->getOperator() == RelationalOperator::GREATER_THAN_OR_EQUALS);
    nameExpr = dynamic_pointer_cast<NameExpression>(relExpr->getRelFactor1());
    REQUIRE(nameExpr->getVarName() == "x");
    nameExpr = dynamic_pointer_cast<NameExpression>(relExpr->getRelFactor2());
    REQUIRE(nameExpr->getVarName() == "y");
    printStmt = dynamic_pointer_cast<PrintStatement>((whileStatement->getStatements())[0]);
    REQUIRE(printStmt->getVariableName() == "longlonglonglonglongNAME");
    readStmt = dynamic_pointer_cast<ReadStatement>((whileStatement->getStatements())[1]);
    REQUIRE(readStmt->getVariableName() == "namewithNUMBERS123053");
}

TEST_CASE("parseWhile - Positive Case - Keywords used as variable name") {
    string rawWhileStatement;
    shared_ptr<RelationalExpression> relExpr;
    shared_ptr<NameExpression> nameExpr;
    shared_ptr<PrintStatement> printStmt;
    shared_ptr<ReadStatement> readStmt;
    shared_ptr<WhileStatement> whileStatement;
    shared_ptr<AssignStatement> assignStatement;

    rawWhileStatement = "while (while > while) {\n"
                               "            print while;\n"
                               "            read while;\n"
                               "          }";

    whileStatement = dynamic_pointer_cast<WhileStatement>(Parser::parseStatement(rawWhileStatement));
    relExpr = dynamic_pointer_cast<RelationalExpression>(whileStatement->getConditionalExpression());
    REQUIRE(relExpr->getOperator() == RelationalOperator::GREATER_THAN);
    nameExpr = dynamic_pointer_cast<NameExpression>(relExpr->getRelFactor1());
    REQUIRE(nameExpr->getVarName() == "while");
    nameExpr = dynamic_pointer_cast<NameExpression>(relExpr->getRelFactor2());
    REQUIRE(nameExpr->getVarName() == "while");
    printStmt = dynamic_pointer_cast<PrintStatement>((whileStatement->getStatements())[0]);
    REQUIRE(printStmt->getVariableName() == "while");
    readStmt = dynamic_pointer_cast<ReadStatement>((whileStatement->getStatements())[1]);
    REQUIRE(readStmt->getVariableName() == "while");


    rawWhileStatement = "while (if > then) {\n"
                        "            print if;\n"
                        "            read else;\n"
                        "            print then;\n"
                        "          }";

    whileStatement = dynamic_pointer_cast<WhileStatement>(Parser::parseStatement(rawWhileStatement));
    relExpr = dynamic_pointer_cast<RelationalExpression>(whileStatement->getConditionalExpression());
    REQUIRE(relExpr->getOperator() == RelationalOperator::GREATER_THAN);
    nameExpr = dynamic_pointer_cast<NameExpression>(relExpr->getRelFactor1());
    REQUIRE(nameExpr->getVarName() == "if");
    nameExpr = dynamic_pointer_cast<NameExpression>(relExpr->getRelFactor2());
    REQUIRE(nameExpr->getVarName() == "then");
    printStmt = dynamic_pointer_cast<PrintStatement>((whileStatement->getStatements())[0]);
    REQUIRE(printStmt->getVariableName() == "if");
    readStmt = dynamic_pointer_cast<ReadStatement>((whileStatement->getStatements())[1]);
    REQUIRE(readStmt->getVariableName() == "else");
    printStmt = dynamic_pointer_cast<PrintStatement>((whileStatement->getStatements())[2]);
    REQUIRE(printStmt->getVariableName() == "then");



    rawWhileStatement = "while (procedure > stop) {\n"
                        "            while = if + then;\n"
                        "            if = while + then;\n"
                        "            then = if + while;\n"
                        "          }";

    whileStatement = dynamic_pointer_cast<WhileStatement>(Parser::parseStatement(rawWhileStatement));
    relExpr = dynamic_pointer_cast<RelationalExpression>(whileStatement->getConditionalExpression());
    REQUIRE(relExpr->getOperator() == RelationalOperator::GREATER_THAN);
    nameExpr = dynamic_pointer_cast<NameExpression>(relExpr->getRelFactor1());
    REQUIRE(nameExpr->getVarName() == "procedure");
    nameExpr = dynamic_pointer_cast<NameExpression>(relExpr->getRelFactor2());
    REQUIRE(nameExpr->getVarName() == "stop");

    assignStatement = dynamic_pointer_cast<AssignStatement>((whileStatement->getStatements())[0]);
    REQUIRE(assignStatement->getVarName() == "while");

    assignStatement = dynamic_pointer_cast<AssignStatement>((whileStatement->getStatements())[1]);
    REQUIRE(assignStatement->getVarName() == "if");

    assignStatement = dynamic_pointer_cast<AssignStatement>((whileStatement->getStatements())[2]);
    REQUIRE(assignStatement->getVarName() == "then");
}

TEST_CASE("parseWhile - Negative Case - Missing semicolon") {
    string str = "while (x == 0) {\n"
                 "\t\t\t\tread x\n"
                 "\t\t}";
    REQUIRE_THROWS_AS(Parser::parseWhile(str), InvalidSyntaxException);
}

TEST_CASE("parseWhile - Negative Case - Extra '}'") {
    string str = "while (!(x == y)) {\n"
                 "\t\t\t\tread x;\n"
                 "\t\t}}";
    REQUIRE_THROWS_AS(Parser::parseWhile(str), InvalidSyntaxException);
}

TEST_CASE("parseWhile - Negative Case - Extra '{'") {
    string str = "while (!(x == y)) {{\n"
                 "\t\t\t\tread x;\n"
                 "\t\t}";
    REQUIRE_THROWS_AS(Parser::parseWhile(str), InvalidSyntaxException);
}

TEST_CASE("parseWhile - Negative Case - Missing '}'") {
    string str = "while (x < 1) {\n"
                 "\t\t\t\tread x;\n";
    REQUIRE_THROWS_AS(Parser::parseWhile(str), InvalidSyntaxException);
}

TEST_CASE("parseWhile - Negative Case - Missing '{'") {
    string str = "while (x < 1) \n"
                 "\t\t\t\tread x;}\n";
    REQUIRE_THROWS_AS(Parser::parseWhile(str), InvalidSyntaxException);
}

TEST_CASE("parseCondExpr - Positive Case") {
    string rawCondExpr = "x <= 5";
    shared_ptr<RelationalExpression> result = dynamic_pointer_cast<RelationalExpression>(Parser::parseCondExpr(rawCondExpr));
    REQUIRE(result->getOperator() == RelationalOperator::LESS_THAN_OR_EQUALS);
    shared_ptr<NameExpression> nameExpr = dynamic_pointer_cast<NameExpression>(result->getRelFactor1());
    REQUIRE(nameExpr->getVarName() == "x");
    shared_ptr<ConstantExpression> constExpr = dynamic_pointer_cast<ConstantExpression>(result->getRelFactor2());
    REQUIRE(constExpr->getValue() == 5);
}

TEST_CASE("parseCondExpr - Positive Case - Annoying Brackets 1") {
    string rawCondExpr = "((((((x)) <= (((5)))))))";
    shared_ptr<RelationalExpression> result = dynamic_pointer_cast<RelationalExpression>(Parser::parseCondExpr(rawCondExpr));
    REQUIRE(result->getOperator() == RelationalOperator::LESS_THAN_OR_EQUALS);
    shared_ptr<NameExpression> nameExpr = dynamic_pointer_cast<NameExpression>(result->getRelFactor1());
    REQUIRE(nameExpr->getVarName() == "x");
    shared_ptr<ConstantExpression> constExpr = dynamic_pointer_cast<ConstantExpression>(result->getRelFactor2());
    REQUIRE(constExpr->getValue() == 5);
}

TEST_CASE("parseCondExpr - Positive Case - Annoying Brackets 2") {
    string str = "((!((((x)) <= (((5)))))))";
    shared_ptr<NotCondition> result = dynamic_pointer_cast<NotCondition>(Parser::parseCondExpr(str));
    shared_ptr<RelationalExpression> condExpr = dynamic_pointer_cast<RelationalExpression>(result->getConditionalExpression());
    REQUIRE(condExpr->getOperator() == RelationalOperator::LESS_THAN_OR_EQUALS);
    shared_ptr<NameExpression> nameExpr = dynamic_pointer_cast<NameExpression>(condExpr->getRelFactor1());
    REQUIRE(nameExpr->getVarName() == "x");
    shared_ptr<ConstantExpression> constExpr = dynamic_pointer_cast<ConstantExpression>(condExpr->getRelFactor2());
    REQUIRE(constExpr->getValue() == 5);
}

TEST_CASE("parseCondExpr - Positive Case - NotCondition") {
    string rawCondExpr = "!(x <= 5)";
    shared_ptr<NotCondition> result = dynamic_pointer_cast<NotCondition>(Parser::parseCondExpr(rawCondExpr));
    shared_ptr<RelationalExpression> condExpr = dynamic_pointer_cast<RelationalExpression>(result->getConditionalExpression());
    REQUIRE(condExpr->getOperator() == RelationalOperator::LESS_THAN_OR_EQUALS);
    shared_ptr<NameExpression> nameExpr = dynamic_pointer_cast<NameExpression>(condExpr->getRelFactor1());
    REQUIRE(nameExpr->getVarName() == "x");
    shared_ptr<ConstantExpression> constExpr = dynamic_pointer_cast<ConstantExpression>(condExpr->getRelFactor2());
    REQUIRE(constExpr->getValue() == 5);
}

TEST_CASE("parseCondExpr - Positive Case - Not expression") {
    string str = "!(y!=1)";
    shared_ptr<NotCondition> notCondition = dynamic_pointer_cast<NotCondition>(Parser::parseCondExpr(str));
    shared_ptr<RelationalExpression> relExpr1 = dynamic_pointer_cast<RelationalExpression>(notCondition->getConditionalExpression());
    REQUIRE(relExpr1->getOperator() == RelationalOperator::NOT_EQUALS);
    shared_ptr<NameExpression> nameExpr = dynamic_pointer_cast<NameExpression>(relExpr1->getRelFactor1());
    REQUIRE(nameExpr->getVarName() == "y");
    shared_ptr<ConstantExpression> constExpr = dynamic_pointer_cast<ConstantExpression>(relExpr1->getRelFactor2());
    REQUIRE(constExpr->getValue() == 1);
}

TEST_CASE("parseCondExpr - Positive Case - Complicated Expressions 1") {
    string rawCondExpr;

    shared_ptr<NotCondition> notCondition;
    shared_ptr<OrCondition> orCondition;
    shared_ptr<AndCondition> andCondition;
    shared_ptr<RelationalExpression> relExpr1;
    shared_ptr<RelationalExpression> relExpr2;
    shared_ptr<NameExpression> nameExpr;
    shared_ptr<ConstantExpression> constExpr;


    rawCondExpr = "!((x <= 5) || (x == 10))";
    notCondition = dynamic_pointer_cast<NotCondition>(Parser::parseCondExpr(rawCondExpr));
    orCondition = dynamic_pointer_cast<OrCondition>(notCondition->getConditionalExpression());
    relExpr1 = dynamic_pointer_cast<RelationalExpression>(orCondition->getConditionalExpression1());
    relExpr2 = dynamic_pointer_cast<RelationalExpression>(orCondition->getConditionalExpression2());

    REQUIRE(relExpr1->getOperator() == RelationalOperator::LESS_THAN_OR_EQUALS);
    nameExpr = dynamic_pointer_cast<NameExpression>(relExpr1->getRelFactor1());
    REQUIRE(nameExpr->getVarName() == "x");
    constExpr = dynamic_pointer_cast<ConstantExpression>(relExpr1->getRelFactor2());
    REQUIRE(constExpr->getValue() == 5);

    REQUIRE(relExpr2->getOperator() == RelationalOperator::EQUALS);
    nameExpr = dynamic_pointer_cast<NameExpression>(relExpr2->getRelFactor1());
    REQUIRE(nameExpr->getVarName() == "x");
    constExpr = dynamic_pointer_cast<ConstantExpression>(relExpr2->getRelFactor2());
    REQUIRE(constExpr->getValue() == 10);


    rawCondExpr = "((x < y) || (x == y)) && !(x > 3)";
    andCondition = dynamic_pointer_cast<AndCondition>(Parser::parseCondExpr(rawCondExpr));
    orCondition = dynamic_pointer_cast<OrCondition>(andCondition->getConditionalExpression1());

    relExpr1 = dynamic_pointer_cast<RelationalExpression>(orCondition->getConditionalExpression1());
    REQUIRE(relExpr1->getOperator() == RelationalOperator::LESS_THAN);
    nameExpr = dynamic_pointer_cast<NameExpression>(relExpr1->getRelFactor1());
    REQUIRE(nameExpr->getVarName() == "x");
    nameExpr = dynamic_pointer_cast<NameExpression>(relExpr1->getRelFactor2());
    REQUIRE(nameExpr->getVarName() == "y");

    relExpr2 = dynamic_pointer_cast<RelationalExpression>(orCondition->getConditionalExpression2());
    REQUIRE(relExpr2->getOperator() == RelationalOperator::EQUALS);
    shared_ptr<NameExpression> nameExpr2 = dynamic_pointer_cast<NameExpression>(relExpr2->getRelFactor1());
    REQUIRE(nameExpr2->getVarName() == "x");
    nameExpr2 = dynamic_pointer_cast<NameExpression>(relExpr2->getRelFactor2());
    REQUIRE(nameExpr2->getVarName() == "y");

    notCondition = dynamic_pointer_cast<NotCondition>(andCondition->getConditionalExpression2());
    relExpr2 = dynamic_pointer_cast<RelationalExpression>(notCondition->getConditionalExpression());
    REQUIRE(relExpr2->getOperator() == RelationalOperator::GREATER_THAN);
    nameExpr = dynamic_pointer_cast<NameExpression>(relExpr2->getRelFactor1());
    REQUIRE(nameExpr->getVarName() == "x");
    constExpr = dynamic_pointer_cast<ConstantExpression>(relExpr2->getRelFactor2());
    REQUIRE(constExpr->getValue() == 3);
}

TEST_CASE("parseCondExpr - Positive Case - Complicated Expressions 2") {
    string rawCondExpr = "(1 >= x ) || (!(procedure == 2147483647))";
    shared_ptr<OrCondition> orCondition = dynamic_pointer_cast<OrCondition>(Parser::parseCondExpr(rawCondExpr));
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

TEST_CASE("parseCondExpr - Positive Case - Complex expression, bad styling") {
    string str = "!  ((x    >    5) &&    (x    < (y   + 3  )))   || ((x    ==3) &&   (y!=    1))";
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

TEST_CASE("parseCondExpr - Negative Case - Missing expression") {
    string str = "(!(x == y) && )";
    REQUIRE_THROWS_AS(Parser::parseCondExpr(str), InvalidSyntaxException);
}

TEST_CASE("parseCondExpr - Negative Case - Extra Bracket") {
    string rawCondExpr;
    rawCondExpr = "(!(x == y)))";
    REQUIRE_THROWS_AS(Parser::parseCondExpr(rawCondExpr), InvalidSyntaxException);

    rawCondExpr = "((!(x == y))";
    REQUIRE_THROWS_AS(Parser::parseCondExpr(rawCondExpr), InvalidSyntaxException);
}

TEST_CASE("parseCondExpr - Negative Case - Invalid conditional operators") {
    string rawCondExpr;
    rawCondExpr = "(!(x == y) & (x != 5))";
    REQUIRE_THROWS_AS(Parser::parseCondExpr(rawCondExpr), InvalidSyntaxException);

    rawCondExpr = "(!(x == y) | (x != 5))";
    REQUIRE_THROWS_AS(Parser::parseCondExpr(rawCondExpr), InvalidSyntaxException);
}

TEST_CASE("parseRelExpr - Positive Case - Simple expressions") {
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

    string expr4 = "x != y + 5";
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

    string expr5 = "something <= 5";
    shared_ptr<RelationalExpression> relExpr5 = Parser::parseRelExpr(expr5);
    REQUIRE(relExpr5->getOperator() == RelationalOperator::LESS_THAN_OR_EQUALS);
    nameExpr = dynamic_pointer_cast<NameExpression>(relExpr5->getRelFactor1());
    constExpr = dynamic_pointer_cast<ConstantExpression>(relExpr5->getRelFactor2());
    REQUIRE(nameExpr->getVarName() == "something");
    REQUIRE(constExpr->getValue() == 5);

    string expr6 = "variable < 3245";
    shared_ptr<RelationalExpression> relExpr6 = Parser::parseRelExpr(expr6);
    REQUIRE(relExpr6->getOperator() == RelationalOperator::LESS_THAN);
    nameExpr = dynamic_pointer_cast<NameExpression>(relExpr6->getRelFactor1());
    constExpr = dynamic_pointer_cast<ConstantExpression>(relExpr6->getRelFactor2());
    REQUIRE(nameExpr->getVarName() == "variable");
    REQUIRE(constExpr->getValue() == 3245);
}

TEST_CASE("parseRelExpr - Positive Case - Annoying brackets 1") {
    // NO Surrounding brackets on outside, refer to function description
    string rawRelExpr;
    rawRelExpr = "(x) <= (((1 + (((2))))))";
    shared_ptr<RelationalExpression> relExpr = Parser::parseRelExpr(rawRelExpr);
    REQUIRE(relExpr->getOperator() == RelationalOperator::LESS_THAN_OR_EQUALS);
    REQUIRE(dynamic_pointer_cast<NameExpression>(relExpr->getRelFactor1())->getVarName() == "x");

    shared_ptr<OperatedExpression> oprExpr = dynamic_pointer_cast<OperatedExpression>(relExpr->getRelFactor2());

    REQUIRE(dynamic_pointer_cast<ConstantExpression>(oprExpr->getExpression1())->getValue() == 1);
    REQUIRE(dynamic_pointer_cast<ConstantExpression>(oprExpr->getExpression2())->getValue() == 2);
}

TEST_CASE("parseRelExpr - Positive Case - Annoying brackets 2") {
    string rawRelExpr = "(((1)))>= (((((1)))%((1))))";
    shared_ptr<RelationalExpression> relExpr = Parser::parseRelExpr(rawRelExpr);
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

TEST_CASE("parseRelExpr - Negative Case") {
    string rawRelExpr;
    rawRelExpr = "x >(=<) 5";
    REQUIRE_THROWS_AS(Parser::parseRelExpr(rawRelExpr), InvalidSyntaxException);

    rawRelExpr = "x >=< 5";
    REQUIRE_THROWS_AS(Parser::parseRelExpr(rawRelExpr), InvalidSyntaxException);

    rawRelExpr = "((x)) === 5";
    REQUIRE_THROWS_AS(Parser::parseRelExpr(rawRelExpr), InvalidSyntaxException);

    rawRelExpr = "x >= x 5";
    REQUIRE_THROWS_AS(Parser::parseRelExpr(rawRelExpr), InvalidSyntaxException);

    rawRelExpr = "x = 5";
    REQUIRE_THROWS_AS(Parser::parseRelExpr(rawRelExpr), InvalidSyntaxException);
}

TEST_CASE("isAssignStatement - Positive Case") {
    string rawAssignStatement;

    rawAssignStatement = "something = xxx";
    REQUIRE(Parser::isAssignStatement(rawAssignStatement));

    rawAssignStatement = "name123456With123456Numbers123456 = variable1 + variable2";
    REQUIRE(Parser::isAssignStatement(rawAssignStatement));

    rawAssignStatement = "while = xxx";
    REQUIRE(Parser::isAssignStatement(rawAssignStatement));

    rawAssignStatement = "if = xxx";
    REQUIRE(Parser::isAssignStatement(rawAssignStatement));

    rawAssignStatement = "procedure = xxx";
    REQUIRE(Parser::isAssignStatement(rawAssignStatement));
}

TEST_CASE("isAssignStatement - Negative Case") {
    string rawAssignStatement;

    rawAssignStatement = "call procedure";
    REQUIRE(!Parser::isAssignStatement(rawAssignStatement));

    rawAssignStatement = "print name";
    REQUIRE(!Parser::isAssignStatement(rawAssignStatement));

    rawAssignStatement = "while ((left + 1 > 2) || (radius + 1 > 2)) {\n"
                         "\t\t\tbreadth = radius - 100;\n"
                         "\t\t\tlength = left + right;\n"
                         "\t\t}";
    REQUIRE(!Parser::isAssignStatement(rawAssignStatement));

    rawAssignStatement = "if (1 > 2) then {\n"
                         "\t\t\t\tprocedure  = 5;\n"
                         "\t\t\t} else {\n"
                         "\t\t\t\tlength = no;\n"
                         "\t\t\t\twhile ((x > 2) || !(y <= 5)) {\n"
                         "\t\t\t\t\tbreadth = while;\n"
                         "\t\t\t\t}\n"
                         "\t\t\t}";
    REQUIRE(!Parser::isAssignStatement(rawAssignStatement));

    rawAssignStatement = "while ((x > 2) || !(y <= 5)) {\n"
                         "\t\t\t\t\tbreadth = while;\n"
                         "\t\t\t\t}";
    REQUIRE(!Parser::isAssignStatement(rawAssignStatement));


    rawAssignStatement = "while (if > then) {\n"
                         "            while = if + then;\n"
                         "            if = while + then;\n"
                         "            then = if + while;\n"
                         "          }";
    REQUIRE(!Parser::isAssignStatement(rawAssignStatement));
}

