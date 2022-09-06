#include "SP/SP.h"
#include "AST/SourceCode.h"
#include "SP/ParserVisitor.h"
#include "catch.hpp"
using namespace std;

TEST_CASE("SP Test 1") {
    string testProgram = "procedure main {\n"
                         "    flag = 0;\n"
                         "    print flag;\n"
                         "    read flag;\n"
                         "\n"
                         "    x = 1;\n"
                         "    y = 2;\n"
                         "    print x;\n"
                         "}";

    SP sp();

    shared_ptr<SourceCode> src = make_shared<SourceCode>(testProgram);
    shared_ptr<ParserVisitor> parserVisitor = make_shared<ParserVisitor>();
    shared_ptr<TNode> AST = src->accept(parserVisitor);

}
