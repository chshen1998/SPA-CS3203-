#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

#include "ParserVisitor.h"
#include "Tokenizer.h"

ParserVisitor::ParserVisitor() {}

void ParserVisitor::visitSourceCode(shared_ptr<SourceCode> sourceCode) {
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