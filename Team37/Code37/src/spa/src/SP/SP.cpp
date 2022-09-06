#include <vector>

using namespace std;

#include "ParserVisitor.h"
#include "AST/SourceCode.h"
#include "SP.h"

shared_ptr<TNode> SP::parse(string program) {
    shared_ptr<ParserVisitor> parserVisitor = make_shared<ParserVisitor>();

    SourceCode sourceCode = SourceCode(program);
    return sourceCode.accept(parserVisitor);
}