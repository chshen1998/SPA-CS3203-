#include <vector>
#include <set>
#include <algorithm>

using namespace std;

#include "Tokenizer.h"
#include "AST/Procedure.h"

// ================================== Utility functions ============================================

const string WHITESPACE = " \n\r\t\f\v";

string ltrim(string s) {
    size_t start = s.find_first_not_of(WHITESPACE);
    return (start == std::string::npos) ? "" : s.substr(start);
}

string rtrim(string s) {
    size_t end = s.find_last_not_of(WHITESPACE);
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

string trim(string s) {
    return rtrim(ltrim(s));
}

bool isNegative (int i) {
    return (i < 0) ;
}

bool isPositive (int i) {
    return (i >= 0);
}

bool isOperatedExpression(string line) {
    vector<string> operators = {"+", "-", "*", "/", "%"};
    vector<int> indexes;
    for (auto o : operators) {
        int i = line.find(o);
        indexes.push_back(i);
    }

    bool isOpExpr = any_of(indexes.begin(), indexes.end(), isPositive);
    if (isOpExpr) {
        Tokenizer:: tokenizeOperatedExpr(line, indexes);
    }
    return isOpExpr;
}

bool isConstant(string line) {
    if (!isOperatedExpression(line)) {
        for (char c : line) {
            if (std::isdigit(c) == 0) {
                return false;
            }
        }
        return true;
    }
}

bool isVariable(string line) {
    return !isConstant(line) && !isOperatedExpression(line);
}

string removeParentheses(string line) {
    string parentheses = "(){}";
    for (auto p : parentheses) {
        line.erase(std::remove(line.begin(), line.end(), p), line.end());
    }
    return line;
}

// ================================== Tokenizing functions ============================================

void Tokenizer:: tokenize(vector<string> lines) {
    shared_ptr<TNode> root = tokenizeProcedure(lines[0]);

    for (int i = 1; i < lines.size(); i++) {
        string l = lines[i];
        tokenizeRead(l, i, root);
        tokenizePrint(l, i, root);
        tokenizeAssignment(l, i, root);
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

void Tokenizer:: tokenizeCondition(string condition) {

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

OperatedExpression Tokenizer:: tokenizeOperatedExpr(string line, vector<int> indexes) {
    string expr = removeParentheses(line);
    // TODO
}

void Tokenizer:: tokenizeAssignment(string line, int lineNo, shared_ptr<TNode> parent) {
    string op = "=";
    vector<string> otherOperators = {"!=", "<=", ">=", "=="};
    vector<int> indexes;
//    set<string> variables;

    int startIdx = line.find(op);
    for (auto o : otherOperators) {
        int i = line.find(o);
        indexes.push_back(i);
    }
    if (startIdx != -1 && (all_of(indexes.begin(), indexes.end(), isNegative))) {
        string leftVar = line.substr(0, startIdx);
        // Parse RHS: could be expression or variable or constant
        string rhs = line.substr(startIdx + 1, string::npos);
        rhs = trim(rhs);
        if (isOperatedExpression(rhs)) {
            // do nothing since isOperatedExpression calls tokenizeOperatedExpr if its is OpExpr
        }
        if (isConstant(rhs)) {
            AssignStatement(parent, lineNo, leftVar, ConstantExpression(stoi(rhs)));
        }
        if (isVariable(rhs)) {
            AssignStatement(parent, lineNo, leftVar, NameExpression(rhs));
        }
    } else if (!all_of(indexes.begin(), indexes.end(), isNegative)){
        cout << "conditional" << endl;
    }
}