#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

#include "../AST/TNode.h"
#include "ParserVisitor.h"
#include "SourceCode.h"

void SP:: parse(string filename) {
    ParserVisitor parserVisitor;

    SourceCode sourceCode = SourceCode(filename);
    sourceCode.accept(parserVisitor);
}