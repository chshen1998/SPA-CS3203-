#include "AST/TNode.h"
#include "SIMPLEAnalyser.cpp"
#include "catch.hpp"
using namespace std;
void require(bool b) {
    REQUIRE(b);
}

TEST_CASE("1st Test") {
    shared_ptr<SIMPLEAnalyser> sp = make_shared<SIMPLEAnalyser>();
    string testProgram = "procedure main {\n"
                  "    flag = 0;\n"
                  "    print flag;\n"
                  "    read flag;\n"
                  "\n"
                  "    x = 1;\n"
                  "    y = 2;\n"
                  "    print x;\n"
                  "}";

    string testQuery = "variable v; select v";

    sp->inputSIMPLE(testProgram);
    string result = sp->executeQuery(testQuery);

    cout << result << endl;
}

