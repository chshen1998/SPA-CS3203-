#include <vector>
#include <algorithm>

using namespace std;

#include "Tokenizer.h"
#include "SP/Utilities/Utils.h"
#include "SP/Utilities/Keywords.h"
#include "InvalidSyntaxException.h"

shared_ptr<ReadStatement> Tokenizer::tokenizeRead(string line, shared_ptr<TNode> parent) {
    string keyword = Keywords::READ;
    int startIdx = line.find(keyword);
    int end = startIdx + keyword.length();
    string varName = line.substr(end, string::npos);
    return make_shared<ReadStatement>(parent, Utils::trim(varName));
}

shared_ptr<PrintStatement> Tokenizer::tokenizePrint(string line, shared_ptr<TNode> parent) {
    string keyword = Keywords::PRINT;
    int startIdx = line.find(keyword);
    int end = startIdx + keyword.length();
    string varName = line.substr(end, string::npos);
    return make_shared<PrintStatement>(parent, Utils::trim(varName));
}

shared_ptr<AssignStatement> Tokenizer::tokenizeAssign(string line, shared_ptr<TNode> parent) {
    string keyword = "=";
    int equalsSignIndex = line.find(keyword);

    // Pull out varName
    string varName = line.substr(0, equalsSignIndex);
    varName = Utils::trim(varName);

    // Pull out relFactor
    string rawRelationalFactor = line.substr(equalsSignIndex + 1);
    rawRelationalFactor = Utils::trim(rawRelationalFactor);

    shared_ptr<RelationalFactor> relFactor = tokenizeRelFactor(rawRelationalFactor);
    shared_ptr<AssignStatement> assignStatement = make_shared<AssignStatement>(parent, varName, relFactor);
    relFactor->setParent(assignStatement);
    return assignStatement;
}

shared_ptr<RelationalFactor> Tokenizer::tokenizeRelFactor(string line) {
    string highPriorityOperators = "*/%";
    string lowPriorityOperators = "+-";

    // Variable used for iteration in while loop
    char nextChar;
    // push_back "left" side expression to this variable
    string expression;
    // Counter in case brackets are encountered. Essentially if this value is not 0 then just keep parsing
    int bracketCounter = 0;
    // Checking if some kind of outer layer of bracket was removed
    bool bracketsDetected = false;

    // Only checks low priority operators, creates operated expression if found
    while(line.length() > 0) {
        nextChar = line[0];
        // Logic to handle "skipping" when bracket counting has started
        if (bracketCounter != 0) {
            if (nextChar == '(') {
                bracketCounter += 1;
            } else if (nextChar == ')') {
                bracketCounter -= 1;
            }

            expression.push_back(nextChar);
            line.erase(0, 1);
            continue;
        }

        if (nextChar == '(') {
            bracketCounter += 1;
            expression.push_back(nextChar);
            line.erase(0, 1);
            bracketsDetected = true;
            continue;
        }

        if (lowPriorityOperators.find(nextChar) == string::npos) {
            expression.push_back(nextChar);
            line.erase(0, 1);
            continue;
        } else {
            // Recursive calls
            shared_ptr<RelationalFactor> leftSide =
                    tokenizeRelFactor(Utils::trim(expression));
            shared_ptr<RelationalFactor> rightSide =
                    tokenizeRelFactor(Utils::trim(line.substr(1)));
            shared_ptr<OperatedExpression> operatedExpression;
            // Create operated expressions
            if (nextChar == '+') {
                operatedExpression = make_shared<OperatedExpression>(
                        nullptr,
                        Operator::ADD,
                        leftSide,
                        rightSide);
            } else if (nextChar == '-') {
                operatedExpression = make_shared<OperatedExpression>(
                        nullptr,
                        Operator::SUBTRACT,
                        leftSide,
                        rightSide);
            }
            leftSide->setParent(operatedExpression);
            rightSide->setParent(operatedExpression);

            return operatedExpression;
        }
    }

    // Reset variables
    line = expression;
    expression.clear();
    bracketCounter = 0;
    // Another while loop that checks high priority stuff
    while(line.length() > 0) {
        nextChar = line[0];
        // Logic to handle "skipping" when bracket counting has started
        if (bracketCounter != 0) {
            if (nextChar == '(') {
                bracketCounter += 1;
            } else if (nextChar == ')') {
                bracketCounter -= 1;
            }
            if (bracketCounter != 0) { // Don't add the final close bracket
                expression.push_back(nextChar);
            }
            line.erase(0, 1);
            continue;
        }

        if (nextChar == '(') {
            bracketCounter += 1;
            line.erase(0, 1);
            bracketsDetected = true;
            continue;
        }

        if (highPriorityOperators.find(nextChar) == string::npos) {
            expression.push_back(nextChar);
            line.erase(0, 1);
            continue;
        } else {
            // Recursive calls
            shared_ptr<RelationalFactor> leftSide =
                    tokenizeRelFactor(Utils::trim(expression));
            shared_ptr<RelationalFactor> rightSide =
                    tokenizeRelFactor(Utils::trim(line.substr(1)));
            shared_ptr<OperatedExpression> operatedExpression;
            // Create operated expressions
            if (nextChar == '*') {
                operatedExpression = make_shared<OperatedExpression>(
                        nullptr,
                        Operator::MULTIPLY,
                        leftSide,
                        rightSide);
            } else if (nextChar == '/') {
                operatedExpression = make_shared<OperatedExpression>(
                        nullptr,
                        Operator::DIVIDE,
                        leftSide,
                        rightSide);
            } else if (nextChar == '%') {
                operatedExpression = make_shared<OperatedExpression>(
                        nullptr,
                        Operator::MODULO,
                        leftSide,
                        rightSide);
            }
            leftSide->setParent(operatedExpression);
            rightSide->setParent(operatedExpression);

            return operatedExpression;
        }
    }

    if (bracketsDetected) {
        return tokenizeRelFactor(Utils::trim(expression));
    }

    // If code reaches this point there should be no more brackets or operators
    // If still no return by this point, then the relFactor is either a variable or a const

    // Reset variables
    line = expression;
    expression.clear();

    string digits = "0123456789";

    // Check if it is a variable
    nextChar = line[0];
    // Must be variable since first letter is not a DIGIT
    if (digits.find(nextChar) == string::npos) {
        return make_shared<NameExpression>(nullptr, line);
    }

    // Make sure everything inside is a digit
    for (char& c: line)  {
        if (digits.find(c) == string::npos) {
            // Either return an error here, or catch the stoi error
            throw InvalidSyntaxException((char *) "Invalid constant for assign statement.");
        }
    }
    // stoi converts std::string to int
    return make_shared<ConstantExpression>(nullptr, stoi(line));

}

vector<shared_ptr<Procedure> > Tokenizer::tokenizeStatements(vector<shared_ptr<Procedure> > procedures, vector<vector<string> > statements) {
    for (int i = 0; i < procedures.size(); i++) {
        shared_ptr<Procedure> procedure = procedures[i];
        for (int j = 0; j < statements[i].size(); j++) {
            shared_ptr<Statement> stmt;
            string s = statements[i][j];
            if (Utils::isRead(s)) {
                stmt = Tokenizer::tokenizeRead(s, procedure);
            }
            if (Utils::isPrint(s)) {
                stmt = Tokenizer::tokenizePrint(s, procedure);
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