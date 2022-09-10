#include <vector>

using namespace std;

#include "ParserVisitor.h"
#include "AST/SourceCode.h"
#include "SP.h"


shared_ptr<SourceCode> SP::parse(string filename) {
    shared_ptr<ParserVisitor> parserVisitor = make_shared<ParserVisitor>();

    SourceCode sourceCode = SourceCode(filename);
    // sourceCode.accept(parserVisitor);

    // Place Holder
    return nullptr;
}

