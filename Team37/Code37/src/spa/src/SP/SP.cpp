#include <vector>

using namespace std;

#include "ParserVisitor.h"
#include "AST/SourceCode.h"
#include "SP.h"

shared_ptr<TNode> SP:: parse(string filename) {
    shared_ptr<ParserVisitor> parserVisitor;

    SourceCode sourceCode = SourceCode(filename);
    return sourceCode.accept(parserVisitor);
}