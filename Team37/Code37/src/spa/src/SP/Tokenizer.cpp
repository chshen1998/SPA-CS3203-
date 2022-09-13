#include <vector>
#include <algorithm>

using namespace std;

#include "Tokenizer.h"
#include "SP/Utilities/Utils.h"
#include "SP/Utilities/Keywords.h"

shared_ptr<ReadStatement> Tokenizer::tokenizeRead(string line, int stmtNo, shared_ptr<TNode> parent) {
    string keyword = Keywords::READ;
    int startIdx = line.find(keyword);
    int end = startIdx + keyword.length();
    string varName = line.substr(end, string::npos);
    return make_shared<ReadStatement>(parent, stmtNo, Utils::trim(varName));
}

shared_ptr<PrintStatement> Tokenizer::tokenizePrint(string line, int stmtNo, shared_ptr<TNode> parent) {
    string keyword = Keywords::PRINT;
    int startIdx = line.find(keyword);
    int end = startIdx + keyword.length();
    string varName = line.substr(end, string::npos);
    return make_shared<PrintStatement>(parent, stmtNo, Utils::trim(varName));
}

vector<shared_ptr<Procedure> > Tokenizer::tokenizeStatements(vector<shared_ptr<Procedure> > procedures, vector<vector<string> > statements) {
    for (int i = 0; i < procedures.size(); i++) {
        shared_ptr<Procedure> procedure = procedures[i];
        for (int j = 0; j < statements[i].size(); j++) {
            shared_ptr<Statement> stmt;
            string s = statements[i][j];
            if (Utils::isRead(s)) {
                stmt = Tokenizer::tokenizeRead(s, j + 1, procedure);
            }
            if (Utils::isPrint(s)) {
                stmt = Tokenizer::tokenizePrint(s, j + 1, procedure);
            }
            procedure->addStatement(stmt);
        }
    }
    return procedures;
}

vector<shared_ptr<Procedure> > Tokenizer::tokenizeProcedure(vector<string> names, vector<vector<string> > statements) {
    vector<shared_ptr<Procedure> > procedures;
    for (int i = 0; i < names.size(); i++) {
        shared_ptr<Procedure> procedure = make_shared<Procedure>(nullptr, names[i]);
        procedures.push_back(procedure);
    }

    return Tokenizer::tokenizeStatements(procedures, statements);
}

shared_ptr<SourceCode> Tokenizer::tokenize(shared_ptr<SourceCode> srcCode, vector<string> names, vector<vector<string> > statements) {
    vector<shared_ptr<Procedure> > procedures = Tokenizer::tokenizeProcedure(names, statements);
    for (auto p: procedures) {
        srcCode->addProcedure(p);
        p->setParent(srcCode);
    }
    return srcCode;
}

//void Tokenizer:: tokenizeCall(string line) {
//    string keyword = "call";
//    int startIdx = line.find(keyword);
//    if (startIdx != string::npos) {
//        int end = startIdx + keyword.length();
//        string procName = line.substr(end, string::npos);
//        CallStatement printStmt = CallStatement(nullptr, ?, procName);
//    }
//}
//
//void Tokenizer:: tokenizeCondition(string condition) {
//    string boolOperators[] = {"&&", "!", "||"};
//    string relOperators[] = {"!=", "<=", ">=", "==", ">", "<"};
//
//}
//
//void Tokenizer:: tokenizeIf(string line, int stmtNo, shared_ptr<TNode> parent) {
//    string ifKeyword = "if";
//    string thenKeyword = "then";
//    int startIf = line.find(ifKeyword);
//    int startThen = line.find(thenKeyword);
//    if (startIf != string::npos && startThen != string::npos) {
//        int end = startIf + ifKeyword.length();
//        string condition = line.substr(end, startThen - end);
//        ConditionalExpression condition =
//        IfStatement ifStmt = IfStatement(parent, stmtNo, condition);
//    }
//}
//
//void Tokenizer:: tokenizeWhile(string line, int stmtNo, shared_ptr<TNode> parent) {
//    string keyword = "while";
//    int startIdx = line.find(keyword);
//    if (startIdx != string::npos) {
//        int end = startIdx + keyword.length();
//        string procName = line.substr(end, string::npos);
//        WhileStatement whileStmt = WhileStatement(nullptr, ?, procName);
//    }
//}

/* Commented out as Compiler throws an error
OperatedExpression Tokenizer:: tokenizeOperatedExpr(string line, vector<int> indexes) {
    string expr = removeParentheses(line);
    // TODO
}
*/

//void Tokenizer:: tokenizeOperatedExprByVar(string line, vector<int> indexes, string lhs, shared_ptr<TNode> parent, int lineNo) { //currently tokenizes into variables only
//    string expr = removeParentheses(line);
//    set<string> expressions;
//
//    indexes.push_back(line.length());
//
//    int start = 0;
//    for (int i = 0; i < indexes.size(); i++) {
//        int end = indexes[i];
//        cout << start << endl;
//        cout << end << endl;
//        string rhs = line.substr(start, end - start);
//        expressions.insert(Utils::trim(removeParentheses(rhs)));
//        start = end + 1;
//    }
//
//    for (auto v : expressions) {
//        if (isConstant(v)) {
//            AssignStatement(parent, lineNo, lhs, make_shared<ConstantExpression>(nullptr, stoi(v)));
//        }
//        if (isVariable(v)) {
//            AssignStatement(parent, lineNo, lhs, make_shared<NameExpression>(nullptr, v));
//        }
//    }
//}

//void Tokenizer:: tokenizeAssignment(string line, int lineNo, shared_ptr<TNode> parent) {
//    string op = "=";
//    string otherOperators[] = {"!=", "<=", ">=", "=="};
//    vector<int> indexes;
//
//    int startIdx = line.find(op);
//    for (auto o : otherOperators) {
//        int i = line.find(o);
//        indexes.push_back(i);
//    }
//    if (startIdx != -1 && (all_of(indexes.begin(), indexes.end(), isNegative))) {
//        string leftVar = line.substr(0, startIdx);
//
//        // Parse RHS: could be expression or variable or constant
//        string rhs = line.substr(startIdx + 1, string::npos);
//        rhs = Utils:: trim(rhs);
//        if (isOperatedExpression(rhs)) {
//            vector<int> opIndexes = getOpIndexes(line);
//            // Tokenizer:: tokenizeOperatedExpr(line, indexes); //TODO
//            Tokenizer:: tokenizeOperatedExprByVar(line, indexes, leftVar, parent, lineNo); // currently tokenizes var only
//        }
//        if (isConstant(rhs)) {
//            AssignStatement(parent, lineNo, leftVar, make_shared<ConstantExpression>(nullptr, stoi(rhs)));
//        }
//        if (isVariable(rhs)) {
//            AssignStatement(parent, lineNo, leftVar, make_shared<NameExpression>(nullptr, rhs));
//        }
//    } else if (!all_of(indexes.begin(), indexes.end(), isNegative)){
//        //TODO: tokenize conditional
//    }
//}