#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

#include "../AST/SourceCode.h"
#include "ParserVisitor.h"
#include "../AST/Procedure.h"
#include "Tokenizer.h"

ParserVisitor::ParserVisitor() {}

void ParserVisitor::visitSourceCode(shared_ptr<SourceCode> sourceCode) {
    string line;
    vector<string> temp;
    vector<string> lines;
    ifstream file (sourceCode->getFileName());
    if (file.is_open()) {
        while (getline(file, line, '\n')) {
            lines.push_back(line);
        }
        for (auto i : lines) {
            stringstream stream (i);
            while (getline(stream, line, '{')){
                temp.push_back(line);
            }
        }
        lines.clear();
        for (auto i : temp) {
            stringstream stream (i);
            while (getline(stream, line, ';')){
                lines.push_back(line);
            }
        }
        file.close();
        // TODO syntax check
        Tokenizer:: tokenize(lines);
    } else {
        cout << "Failed to open file" << endl;
    }
}

//    void ParserVisitor:: visitTokenGroup(TokenGroup tokenGroup) {
//        // TODO
//    }
//    void ParserVisitor:: visitProcedureList(ProcedureList procedureList) {
//        // TODO
//    }
//    void ParserVisitor:: visitStatementList(StatementList statementList) {
//        // TODO
//    }
//    void ParserVisitor:: visitToken(Token token) {
//        // TODO
//    }