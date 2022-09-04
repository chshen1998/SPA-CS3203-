#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

#include "../AST/TNode.h"
#include "ParserVisitor.h"

void ParserVisitor::visitSourceCode(shared_ptr<SourceCode> sourceCode) {
    string line;
    vector<string> temp;
    vector<string> lines;
    ifstream file (sourceCode->filename);
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