#include <vector>

using namespace std;

#include "AST/SourceCode.h"
#include "SP.h"
#include "Tokenizer.h"
#include "Parser.h"
#include "Utilities/Utils.h"

shared_ptr<SourceCode> SP::parse(string filepath) {
    string code = Utils::fileToString(filepath);
    vector<string> procedures;
    procedures = Parser::extractProcedures(code, procedures);
    vector<string> procNames = Parser::extractProcNames(procedures);

    vector<vector<string> > statementLists;
    for (auto p: procedures) {
        vector<string> stmts = Parser::extractStatements(p);
        statementLists.push_back(stmts);
    }

    shared_ptr<SourceCode> sourceCode = make_shared<SourceCode>(filepath);
    sourceCode = Tokenizer::tokenize(sourceCode, procNames, statementLists);
    return sourceCode;
}