#include <vector>
#include <set>
#include <algorithm>

using namespace std;

#include "Tokenizer.h"
#include "Utils.h"

// ================================== Utility functions ============================================

bool isNegative (int i) {
    return (i < 0) ;
}

bool isPositive (int i) {
    return (i >= 0);
}

vector<int> getOpIndexes(string line) {
    string operators[] = {"+", "-", "*", "/", "%"};
    vector<int> indexes;
    for (auto o : operators) {
        int i = line.find(o);
        indexes.push_back(i);
    }
    return indexes;
}

bool isOperatedExpression(string line) {
    vector<int> indexes = getOpIndexes(line);
    bool isOpExpr = any_of(indexes.begin(), indexes.end(), isPositive);
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

bool isRead(string line) {
    string keyword = "read";
    int startIdx = line.find(keyword);
    return (startIdx != -1);
}

bool isPrint(string line) {
    string keyword = "print";
    int startIdx = line.find(keyword);
    return (startIdx != -1);
}

// ================================== Tokenizing functions ============================================
shared_ptr<ReadStatement> Tokenizer:: tokenizeRead(string line, int stmtNo, shared_ptr<TNode> parent) {
    string keyword = "read";
    int startIdx = line.find(keyword);
    int end = startIdx + keyword.length();
    string varName = line.substr(end, string::npos);
    return make_shared<ReadStatement>(parent, stmtNo, Utils::trim(varName));
}

shared_ptr<PrintStatement> Tokenizer:: tokenizePrint(string line, int stmtNo, shared_ptr<TNode> parent) {
    string keyword = "print";
    int startIdx = line.find(keyword);
    int end = startIdx + keyword.length();
    string varName = line.substr(end, string::npos);
    return make_shared<PrintStatement>(parent, stmtNo, Utils::trim(varName));
}

/**
 * Creates statement objects and sets procedure as the parent node
 * @param procedures vector containing all procedures of program
 * @param statements vector containing statement lists as strings for each procedure
 * @return vector containing procedures with corresponding statement objects
 */
vector<shared_ptr<Procedure> > Tokenizer:: tokenizeStatements(vector<shared_ptr<Procedure> > procedures, vector<vector<string> > statements) {
    for (int i = 0; i < procedures.size(); i++) {
        shared_ptr<Procedure> procedure = procedures[i];
        for (int j = 0; j < statements[i].size(); j++) {
            shared_ptr<Statement> stmt;
            string s = statements[i][j];
            if (isRead(s)) {
                stmt = Tokenizer::tokenizeRead(s, j + 1, procedure);
            }
            if (isPrint(s)) {
                stmt = Tokenizer::tokenizePrint(s, j + 1, procedure);
            }
            procedure->addStatement(stmt);
        }
    }
    return procedures;
}

/**
 * Converts procedure as a string to Procedure object.
 * @param names vector containing all names of procedures in strings
 * @param statements vector containing vector of statements corresponding to each procedure
 * @return vector containing procedures.
 */
vector<shared_ptr<Procedure> > Tokenizer:: tokenizeProcedure(vector<string> names, vector<vector<string> > statements) {
    vector<shared_ptr<Procedure> > procedures;
    for (int i = 0; i < names.size(); i++) {
        shared_ptr<Procedure> procedure = make_shared<Procedure>(nullptr, names[i]);
        procedures.push_back(procedure);
    }

    return Tokenizer::tokenizeStatements(procedures, statements);
}

/**
 * Tokenizes SourceCode and sets SourceCode as parent to all corresponding procedures.
 * @param srcCode sourcecode object that acts as root node
 * @param names vector of all procedure names of program
 * @param statements vector of vector containing statements as strings for each corresponding procedure
 * @return sourcecode node that now has all procedures in its procedure list
 */
shared_ptr<SourceCode> Tokenizer:: tokenize(shared_ptr<SourceCode> srcCode, vector<string> names, vector<vector<string> > statements) {
    vector<shared_ptr<Procedure> > procedures = Tokenizer::tokenizeProcedure(names, statements);
    for (auto p: procedures) {
        srcCode->addProcedure(p);
        p->setParent(srcCode);
    }
    return srcCode;
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
    string boolOperators[] = {"&&", "!", "||"};
    string relOperators[] = {"!=", "<=", ">=", "==", ">", "<"};

}

void Tokenizer:: tokenizeIf(string line, int stmtNo, shared_ptr<TNode> parent) {
    string ifKeyword = "if";
    string thenKeyword = "then";
    int startIf = line.find(ifKeyword);
    int startThen = line.find(thenKeyword);
    if (startIf != string::npos && startThen != string::npos) {
        int end = startIf + ifKeyword.length();
        string condition = line.substr(end, startThen - end);
//        ConditionalExpression condition =
//        IfStatement ifStmt = IfStatement(parent, stmtNo, condition);
    }
}

void Tokenizer:: tokenizeWhile(string line, int stmtNo, shared_ptr<TNode> parent) {
    string keyword = "while";
    int startIdx = line.find(keyword);
    if (startIdx != string::npos) {
        int end = startIdx + keyword.length();
        string procName = line.substr(end, string::npos);
//        WhileStatement whileStmt = WhileStatement(nullptr, ?, procName);
    }
}

/* Commented out as Compiler throws an error
OperatedExpression Tokenizer:: tokenizeOperatedExpr(string line, vector<int> indexes) {
    string expr = removeParentheses(line);
    // TODO
}
*/

void Tokenizer:: tokenizeOperatedExprByVar(string line, vector<int> indexes, string lhs, shared_ptr<TNode> parent, int lineNo) { //currently tokenizes into variables only
    string expr = removeParentheses(line);
    set<string> expressions;

    indexes.push_back(line.length());

    int start = 0;
    for (int i = 0; i < indexes.size(); i++) {
        int end = indexes[i];
        cout << start << endl;
        cout << end << endl;
        string rhs = line.substr(start, end - start);
        expressions.insert(Utils::trim(removeParentheses(rhs)));
        start = end + 1;
    }

    for (auto v : expressions) {
        if (isConstant(v)) {
            AssignStatement(parent, lineNo, lhs, make_shared<ConstantExpression>(nullptr, stoi(v)));
        }
        if (isVariable(v)) {
            AssignStatement(parent, lineNo, lhs, make_shared<NameExpression>(nullptr, v));
        }
    }
}

void Tokenizer:: tokenizeAssignment(string line, int lineNo, shared_ptr<TNode> parent) {
    string op = "=";
    string otherOperators[] = {"!=", "<=", ">=", "=="};
    vector<int> indexes;

    int startIdx = line.find(op);
    for (auto o : otherOperators) {
        int i = line.find(o);
        indexes.push_back(i);
    }
    if (startIdx != -1 && (all_of(indexes.begin(), indexes.end(), isNegative))) {
        string leftVar = line.substr(0, startIdx);

        // Parse RHS: could be expression or variable or constant
        string rhs = line.substr(startIdx + 1, string::npos);
        rhs = Utils:: trim(rhs);
        if (isOperatedExpression(rhs)) {
            vector<int> opIndexes = getOpIndexes(line);
            // Tokenizer:: tokenizeOperatedExpr(line, indexes); //TODO
            Tokenizer:: tokenizeOperatedExprByVar(line, indexes, leftVar, parent, lineNo); // currently tokenizes var only
        }
        if (isConstant(rhs)) {
            AssignStatement(parent, lineNo, leftVar, make_shared<ConstantExpression>(nullptr, stoi(rhs)));
        }
        if (isVariable(rhs)) {
            AssignStatement(parent, lineNo, leftVar, make_shared<NameExpression>(nullptr, rhs));
        }
    } else if (!all_of(indexes.begin(), indexes.end(), isNegative)){
        //TODO: tokenize conditional
    }
}