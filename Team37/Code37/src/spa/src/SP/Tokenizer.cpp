#include <vector>
#include <set>

using namespace std;

#include "Tokenizer.h"
#include "AST/Procedure.h"

void Tokenizer:: tokenize(vector<string> lines) {
    shared_ptr<TNode> root = nullptr;
    for (auto l : lines) {
//        vector<string> keywords = {"read", "print", "call", "while", "if"};
//        vector<string> operators = {"=", "+", "-", "*", "/", "%", "!=", ">", "<", "<=", ">=", "=="};
        if (root == nullptr) {
            tokenizeRead(l);
            tokenizePrint(l);
            tokenizeAssignment(l);
        }

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

void Tokenizer:: tokenizeRead(string line) {
    string keyword = "read";
    int startIdx = line.find(keyword);
    if (startIdx != string::npos) {
        int end = startIdx + keyword.length();
        string varName = line.substr(end, string::npos);
//        ReadStatement readStmt = ReadStatement(nullptr, ?, varName);
    }
}

void Tokenizer:: tokenizePrint(string line) {
    string keyword = "print";
    int startIdx = line.find(keyword);
    if (startIdx != string::npos) {
        int end = startIdx + keyword.length();
        string varName = line.substr(end, string::npos);
//        PrintStatement printStmt = PrintStatement(nullptr, ?, varName);
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