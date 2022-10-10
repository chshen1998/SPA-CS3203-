#include <vector>

using namespace std;

#include "AST/SourceCode.h"
#include "AST/Statement/Statement.h"
#include "SP.h"
#include "Parser.h"
#include "Utilities/Utils.h"
#include "InvalidSyntaxException.h"
#include "SP/Utilities/CallLoopChecker.h"

shared_ptr<SourceCode> SP::parse(string filepath) {
    string srcCode = Utils::fileToString(filepath);
    shared_ptr<SourceCode> AST =  Parser::parseSourceCode(srcCode, filepath);
    Statement::resetLineNumCount();

    // Throw error if there are any
    CallLoopChecker::checkCallLoop(AST);

    return AST;
}
