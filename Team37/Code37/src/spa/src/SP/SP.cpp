#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

#include "ParserVisitor.h"
#include "AST/SourceCode.h"

void SP:: parse(string filename) {
    shared_ptr<ParserVisitor> parserVisitor;

    SourceCode sourceCode = SourceCode(filename);
    sourceCode.accept(parserVisitor);
}