#include "SP/Parser.h"
#include "SP/InvalidSyntaxException.h"

#include <catch.hpp>
#include <string>
#include <vector>

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
                   "\t\t\tif (true) then {\n"
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
                 "\t\t\tif (true) then {\n"
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
    result = Parser::extractProcName(rawProcedure);
    REQUIRE(result == "somethingHmm");

    rawProcedure = "procedure nestedBlocks {\n"
                   "\tif (iter != 1) then {\n"
                   "\t\titer = iter + 1; \n"
                   "\t\tlength = radius * 10;\n"
                   "\t\tbreadth = radius + 10;\n"
                   "\t} else {\n"
                   "\t\twhile ((left + 1 > 2) || (radius + 1 > 2)) {\n"
                   "\t\t\tbreadth = radius - 100;\n"
                   "\t\t\tlength = left + right;\n"
                   "\t\t\tif (true) then {\n"
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
    result = Parser::extractProcName(rawProcedure);
    REQUIRE(result == "nestedBlocks");
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
                    "\t\t\tif (true) then {\n"
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
                         "\t\t\tif (true) then {\n"
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

TEST_CASE("extractConditionalExpr - Positive Case") {
    string ifStatementStr = "if ((x != 9) && (x <= y))";
    string expected = "(x != 9) && (x <= y)";
    string result = Parser::extractConditionalExpr(ifStatementStr);
    REQUIRE(result == expected);
}

// TODO: Shift more test cases into more organized files, continue writing more unit tests
