#include <vector>

using namespace std;

#include "Parser.h"
#include "Tokenizer.h"
#include "Utilities/Keywords.h"
#include "Utilities/Utils.h"
#include "AST/SourceCode.h"
#include "InvalidSyntaxException.h"
#include "Utilities/ConditionalExpressionNode.h"

//TODO: pass in pointer to procedures instead of pass by value
vector<string> Parser::extractProcedures(string srcCode, vector<string> procedures) {
    string keyword = Keywords:: PROCEDURE;

    int startIdx = srcCode.find(keyword);
    if (startIdx != -1) {
        int len = startIdx + keyword.length();
        string remaining = srcCode.substr(len, string::npos);

        int next = remaining.find(keyword);
        if (next != -1) {
            string statements = remaining.substr(0, next);
            string nextBlock = remaining.substr(next, string:: npos);

            string p = keyword + statements;
            procedures.push_back(p);
            procedures = extractProcedures(nextBlock, procedures);
        } else {
            procedures.push_back(keyword + remaining);
        }
    }
    return procedures;
}

string Parser::extractProcName(string procedure) {
    string keyword = Keywords::PROCEDURE;

    int procId = procedure.find(keyword);
    procId = procId + keyword.length();
    string bracket = "{";
    int bracketId = procedure.find(bracket);
    string name = procedure.substr(procId, bracketId - procId);
    return Utils::trim(name);
}

vector<string> Parser::extractStatements(string procedure) {
    vector<string> statements;
    //TODO: modify according to refactoring
    return statements;
}

string Parser::extractConditionalExpr(string block, size_t firstEgyptianOpen) {
    string stmtStr = block.substr(0, firstEgyptianOpen);
    size_t condExprStart = stmtStr.find_first_of(Keywords::OPEN_BRACKET) + 1;
    size_t condExprEnd = stmtStr.find_last_of(Keywords::CLOSE_BRACKET);
    size_t condExprLength = condExprEnd - condExprStart;
    return Utils::trim(stmtStr.substr(condExprStart, condExprLength));
}

string Parser::extractStatementBlock(string block, size_t firstEgyptianOpen) {
    size_t stmtLstStart = firstEgyptianOpen + 1;
    size_t stmtLstEnd = block.find_last_of(Keywords::CLOSE_EGYPTIAN);
    size_t stmtLstLength = stmtLstEnd - stmtLstStart;
    return Utils::trim(block.substr(stmtLstStart, stmtLstLength));
}

shared_ptr<ConditionalExpression> Parser::parseCondExpr(string condExprStr, shared_ptr<TNode> parent) {
    condExprStr = Utils::trim(condExprStr);
    int NotOpIdx = condExprStr.find_first_of(Keywords::NOT_OPERATOR);
    int NotEqOpIdx = condExprStr.find(Keywords::NOT_EQUALS);
    if (NotEqOpIdx != NotOpIdx && NotOpIdx == 0) {
        // NOT statement - ! operator is the first character of the trimmed string
        string innerCondExpr = condExprStr.substr(NotOpIdx + 1, string::npos);
        shared_ptr<ConditionalExpression> condExpr = Parser::parseCondExpr(innerCondExpr, nullptr);
        shared_ptr<NotCondition> NotCondExpr = make_shared<NotCondition>(parent, condExpr);
        condExpr->setParent(NotCondExpr);
        return NotCondExpr;
    }

    // if no operators found, throw error, invalid conditional expression
    throw InvalidSyntaxException((char *)"No operators found. Invalid conditional expression.");
    return nullptr;
}

shared_ptr<IfStatement> Parser::parseIfElse(string ifElseBlock, int stmtNo, shared_ptr<TNode> parent) {
    size_t elseIndex = ifElseBlock.find(Keywords::ELSE);
    size_t thenIndex = ifElseBlock.find(Keywords::THEN);

    // Throw exceptions for missing else or then statements, will be caught in SP::Parser()
    if ((int) elseIndex == -1) {
        throw InvalidSyntaxException((char *)"Syntax error: if-else must have else block.");
    }
    if ((int) thenIndex == -1) {
        throw InvalidSyntaxException((char *)"Syntax error: if statement must have 'then'.");
    }
    string ifBlock = ifElseBlock.substr(0, elseIndex);
    string elseBlock = ifElseBlock.substr(elseIndex, string::npos);

    size_t firstEgyptianOpen = ifElseBlock.find_first_of(Keywords::OPEN_EGYPTIAN);
    string condExpr = Parser::extractConditionalExpr(ifElseBlock, firstEgyptianOpen);
    shared_ptr<ConditionalExpression> condExprNode = Parser::parseCondExpr(condExpr, nullptr);
    shared_ptr<IfStatement> ifNode = make_shared<IfStatement>(parent, stmtNo, condExprNode);
    condExprNode->setParent(ifNode);
    string ifStmtBlock = Parser::extractStatementBlock(ifBlock, firstEgyptianOpen);
    vector<string> stmts = Parser::extractStatements(ifStmtBlock);

    for (auto s: stmts) {
        shared_ptr<Statement> statement = Parser::parseStatement(s, ifNode);
        ifNode->addThenStatement(statement);
        statement->setParent(ifNode);
    }

    firstEgyptianOpen = elseBlock.find_first_of(Keywords::OPEN_EGYPTIAN);
    string elseStmtBlock = Parser::extractStatementBlock(elseBlock, firstEgyptianOpen);
    stmts = Parser::extractStatements(elseStmtBlock);
    for (auto s: stmts) {
        shared_ptr<Statement> statement = Parser::parseStatement(s, ifNode);
        ifNode->addElseStatement(statement);
        statement->setParent(ifNode);
    }
//    parent->addStatement(ifNode); //TODO TNode has no addStatement method
//    ifNode->setParent(parent); //not sure if this is done in parseStatement
    return ifNode;
}

shared_ptr<WhileStatement> Parser::parseWhile(string whileBlock, int stmtNo, shared_ptr<TNode> parent) {
    size_t firstEgyptianOpen = whileBlock.find_first_of(Keywords::OPEN_EGYPTIAN);
    string condExpr = Parser::extractConditionalExpr(whileBlock, firstEgyptianOpen);

    shared_ptr<ConditionalExpression> condExprNode = Parser::parseCondExpr(condExpr, nullptr);
    shared_ptr<WhileStatement> whileStatement = make_shared<WhileStatement>(parent, stmtNo, condExprNode);
    condExprNode->setParent(whileStatement);

    string stmtsBlock = Parser::extractStatementBlock(whileBlock, firstEgyptianOpen);
    vector<string> stmts = Parser::extractStatements(stmtsBlock);
    for (auto s:stmts) {
        shared_ptr<Statement> statement = Parser::parseStatement(s, whileStatement);
        whileStatement->addStatement(statement);
        statement->setParent(whileStatement);
    }
//    parent->addStatement(whileStatement); //TODO not sure if this is done in parseStatement
//    whileStatement->setParent(parent);
    return whileStatement;
}

shared_ptr<Statement> Parser::parseStatement(string statement, shared_ptr<TNode> parentNode) {
    // TODO: split into cases for (print, read, call, if-else, while, assign)
}

shared_ptr<Procedure> Parser::parseProcedure(string procedure, shared_ptr<SourceCode> srcCodeNode) {
    vector<string> statementList = Parser::extractStatements(procedure);
    string procedureName = Parser::extractProcName(procedure);
    shared_ptr<Procedure> procedureNode = make_shared<Procedure>(srcCodeNode, procedureName);
    for (auto stmt : statementList) {
        shared_ptr<Statement> statement = Parser::parseStatement(stmt, procedureNode);
        procedureNode->addStatement(statement);
        statement->setParent(procedureNode);
    }
    return procedureNode;
}

shared_ptr<SourceCode> Parser::parseSourceCode(string srcCode, string filepath) {
    shared_ptr<SourceCode> sourceCodeNode = make_shared<SourceCode>(filepath);
    vector<string> procedureList;
    procedureList = Parser::extractProcedures(srcCode, procedureList);
    // call parseProcedure on each procedure
    for (auto p: procedureList) {
        shared_ptr<Procedure> procedure = Parser::parseProcedure(p, sourceCodeNode);
        sourceCodeNode->addProcedure(procedure);
        procedure->setParent(sourceCodeNode);
    }
    return sourceCodeNode;
}

// extract statements w bracket pairing
//vector<string> SP:: extractStatements(string procedure) {
//    vector<string> statements;
//    vector<int> openIndexes;
//    vector<int> closeIndexes;
//    openIndexes = Utils::getOpenIndexes(procedure, openIndexes, 0);
//    closeIndexes = Utils::getClosedIndexes(procedure, closeIndexes, 0);
//
//    if (openIndexes.size() != closeIndexes.size()){
//        // throw error
//        if(openIndexes.size() < closeIndexes.size()) {
//            cout << "Missing {" << endl;
//        } else {
//            cout << "Missing }" << endl;
//        }
//    } else {
//        vector<vector<int> > allPairs = Utils::getSets(openIndexes, closeIndexes);
//        sort(allPairs.begin(), allPairs.end());
//        allPairs.erase(allPairs.begin()); // remove first set of brackets as it belongs to procedure
//        // group if/else/while
//    }
//}