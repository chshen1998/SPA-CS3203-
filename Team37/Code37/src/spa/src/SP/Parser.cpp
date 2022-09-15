#include <vector>

using namespace std;

#include "Parser.h"
#include "Tokenizer.h"
#include "Utilities/Keywords.h"
#include "Utilities/Utils.h"
#include "AST/SourceCode.h"

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

shared_ptr<ConditionalExpression> Parser::parseCondExpr(string condExprStr) {
    //TODO
}

shared_ptr<IfStatement> Parser::parseIfElse(string ifElse) {
    //TODO
}

shared_ptr<WhileStatement> Parser::parseWhile(string whileBlock, int stmtNo, shared_ptr<TNode> parent) {
    size_t firstEgyptianOpen = whileBlock.find_first_of(Keywords::OPEN_EGYPTIAN);

    string whileStr = whileBlock.substr(0, firstEgyptianOpen);
    size_t condExprStart = whileStr.find_first_of(Keywords::OPEN_BRACKET) + 1;
    size_t condExprEnd = whileStr.find_last_of(Keywords::CLOSE_BRACKET);
    size_t condExprLength = condExprEnd - condExprStart;
    string condExpr = Utils::trim(whileStr.substr(condExprStart, condExprLength));

    shared_ptr<ConditionalExpression> condExprNode = Parser::parseCondExpr(condExpr);
    shared_ptr<WhileStatement> whileStatement = make_shared<WhileStatement>(parent, stmtNo, condExprNode);
    condExprNode->setParent(whileStatement);

    size_t stmtLstStart = firstEgyptianOpen + 1;
    size_t stmtLstEnd = whileBlock.find_last_of(Keywords::CLOSE_EGYPTIAN);
    size_t stmtLstLength = stmtLstEnd - stmtLstStart;
    string stmtsBlock = Utils::trim(whileBlock.substr(stmtLstStart, stmtLstLength));
    vector<string> stmts = Parser::extractStatements(stmtsBlock);
    for (auto s:stmts) {
        shared_ptr<Statement> statement = Parser::parseStatement(s, whileStatement);
        whileStatement->addStatement(statement);
        statement->setParent(whileStatement);
    }
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