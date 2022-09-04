#include <vector>
#include <set>

using namespace std;

#include "Tokenizer.h"
#include "AST/Procedure.h"

void Tokenizer:: tokenize(vector<string> lines) {
    shared_ptr<TNode> root = tokenizeProcedure(lines[0]);

    for (int i = 1; i < lines.size(); i++) {
        string l = lines[i];
        tokenizeRead(l, i, root);
        tokenizePrint(l, i, root);
//        tokenizeAssignment(l);
    }
}

shared_ptr<Procedure> Tokenizer:: tokenizeProcedure(string line, int startIdx, string keyword) {
    int end = startIdx + keyword.length();
    string name = line.substr(end, string::npos);
    shared_ptr<Procedure> procedure = make_shared<Procedure>(Procedure(nullptr, name));
    return procedure;
}

shared_ptr<Procedure> Tokenizer:: tokenizeProcedure(string line) {
    string keyword = "procedure";
    int startIdx = line.find(keyword);
    if (startIdx != string::npos) {
        int end = startIdx + keyword.length();
        string name = line.substr(end, string::npos);
        shared_ptr<Procedure> procedure = make_shared<Procedure>(Procedure(nullptr, name));
        return procedure;
    }
}

void Tokenizer:: tokenizeRead(string line, int stmtNo, shared_ptr<TNode> parent) {
    string keyword = "read";
    int startIdx = line.find(keyword);
    if (startIdx != string::npos) {
        int end = startIdx + keyword.length();
        string varName = line.substr(end, string::npos);
//        shared_ptr<ReadStatement> readStmt = make_shared<ReadStatement>(ReadStatement(parent, stmtNo, varName));
//        return readStmt;
        ReadStatement(parent, stmtNo, varName);
    }
}

void Tokenizer:: tokenizePrint(string line, int stmtNo, shared_ptr<TNode> parent) {
    string keyword = "print";
    int startIdx = line.find(keyword);
    if (startIdx != string::npos) {
        int end = startIdx + keyword.length();
        string varName = line.substr(end, string::npos);
//        shared_ptr<PrintStatement> printStmt = make_shared<PrintStatement>(PrintStatement(parent, stmtNo, varName));
//        return printStmt;
        PrintStatement(parent, stmtNo, varName);
    }
}

void Tokenizer:: tokenizeCall(string line) {
    string keyword = "call";
    int startIdx = line.find(keyword);
    if (startIdx != string::npos) {
        int end = startIdx + keyword.length();
        string procName = line.substr(end, string::npos);
//        CallStatement printStmt = CallStatement(nullptr, ?, procName);
    }
}

void Tokenizer:: tokenizeIf(string line) {
    string ifKeyword = "if";
    string thenKeyword = "then";
    int startIf = line.find(ifKeyword);
    int startThen = line.find(thenKeyword);
    if (startIf != string::npos && startThen != string::npos) {
        int end = startIf + ifKeyword.length();
        string condition = line.substr(end, startThen - end);
//        IfStatement ifStmt = IfStatement(nullptr, ?, procName);
    }
}

void Tokenizer:: tokenizeWhile(string line) {
    string keyword = "while";
    int startIdx = line.find(keyword);
    if (startIdx != string::npos) {
        int end = startIdx + keyword.length();
        string procName = line.substr(end, string::npos);
//        WhileStatement whileStmt = WhileStatement(nullptr, ?, procName);
    }
}

void Tokenizer:: tokenizeAssignment(string line) {
    string op = "=";
    vector<string> otherOperators = {"!=", "<=", ">=", "=="};
    vector<int> indexes;
    set<string> variables;

    int startIdx = line.find(op);
    for (auto o : otherOperators) {
        int i = line.find(o);
        indexes.push_back(i);
    }
    if (startIdx != string::npos && !(std::count(indexes.begin(), indexes.end(), string::npos))) {
        string leftVar = line.substr(0, startIdx);
        // TODO parse RHS: could be expression or variable
    }
}