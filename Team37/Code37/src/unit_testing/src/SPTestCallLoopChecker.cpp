#include <catch.hpp>
#include <string>
#include <vector>

#include "SP/Utilities/CallLoopChecker.h"
#include "SP/Parser.h"
#include "SP/InvalidSyntaxException.h"

/*
 * Naming Convention:
 * functionName - Positive/Negative Case - (Opt: Others)
 *
 * E.g.
 * tokenizeRelFactor - Negative Case - Very Nested Blocks
 */

TEST_CASE("checkCallLoop - Positive Case") {
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

    shared_ptr<SourceCode> AST = Parser::parseSourceCode(rawSourceCode, "filename");
    REQUIRE(!CallLoopChecker::checkCallLoop(AST));
}

TEST_CASE("checkCallLoop - Negative Case - Recursive call") {
    string rawSourceCode = "procedure main {\n"
                           "\tprint = x;\n"
                           "\tread x;\n"
                           "\tleft = x;\n"
                           "\tright = left + 2;\n"
                           "\tread temp;\n"
                           "\tradius = x + left;\n"
                           "\ttemp = temp + 2;\n"
                           "\tradius1 = x + left;\n"
                           "\tcall main;\n"
                           "}";

    shared_ptr<SourceCode> AST = Parser::parseSourceCode(rawSourceCode, "filename");
    REQUIRE_THROWS_AS(CallLoopChecker::checkCallLoop(AST), InvalidSyntaxException);
}

TEST_CASE("checkCallLoop - Negative Case - Procedure call loop") {
    string rawSourceCode = "procedure main {\n"
                           "\tprint = x;\n"
                           "\tread x;\n"
                           "\tleft = x;\n"
                           "\tright = left + 2;\n"
                           "\tread temp;\n"
                           "\tradius = x + left;\n"
                           "\ttemp = temp + 2;\n"
                           "\tradius1 = x + left;\n"
                           "\tcall somethingHmm;\n"
                           "}\n"
                           "\n"
                           "procedure somethingHmm {\n"
                           "\twhile (iter <=  5) {\n"
                           "\t\tx = right + 3 + 2 + left;\n"
                           "\t\ttemp = temp + 7;\n"
                           "\t\tcall main;\n"
                           "\t}\n"
                           "\tx = 5;\n"
                           "\tif (iter != 1) then {\n"
                           "\t\titer = iter + 1; \n"
                           "\t\tlength = radius * 10;\n"
                           "\t\tbreadth = radius + 10;\n"
                           "\t} else {\n"
                           "\t\twhile ((left + 1 > 2) || (radius + 1 > 2)) {\n"
                           "\t\t\tlength = left + right;\n"
                           "\t\t}\n"
                           "\t}\n"
                           "}";

    shared_ptr<SourceCode> AST = Parser::parseSourceCode(rawSourceCode, "filename");
    REQUIRE_THROWS_AS(CallLoopChecker::checkCallLoop(AST), InvalidSyntaxException);
}

TEST_CASE("checkCallLoop - Negative Case - Calling non-existent procedure") {
    string rawSourceCode = "procedure main {\n"
                           "\tprint = x;\n"
                           "\tread x;\n"
                           "\tleft = x;\n"
                           "\tright = left + 2;\n"
                           "\tread temp;\n"
                           "\tradius = x + left;\n"
                           "\ttemp = temp + 2;\n"
                           "\tradius1 = x + left;\n"
                           "\tcall somethingHmm;\n"
                           "}";

    shared_ptr<SourceCode> AST = Parser::parseSourceCode(rawSourceCode, "filename");
    REQUIRE_THROWS_AS(CallLoopChecker::checkCallLoop(AST), InvalidSyntaxException);
}

TEST_CASE("checkCallLoop - Negative Case - Repeated procedure name") {
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
                           "procedure main {\n"
                           "\tprint x;\n"
                           "}";

    shared_ptr<SourceCode> AST = Parser::parseSourceCode(rawSourceCode, "filename");
    REQUIRE_THROWS_AS(CallLoopChecker::checkCallLoop(AST), InvalidSyntaxException);
}
