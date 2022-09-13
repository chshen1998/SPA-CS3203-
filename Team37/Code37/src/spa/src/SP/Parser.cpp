#include <sstream>
#include <vector>

using namespace std;

#include "Parser.h"
#include "Tokenizer.h"
#include "Utilities/Keywords.h"
#include "Utilities/Utils.h"
#include "AST/SourceCode.h"

vector<string> Parser:: extractProcedures(string srcCode, vector<string> procedures) {
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

vector<string> Parser:: extractProcNames(vector<string> procedures) {
    string keyword = Keywords::PROCEDURE;
    vector<string> names;

    for (auto proc : procedures) {
        int procId = proc.find(keyword);
        procId = procId + keyword.length();
        string bracket = "{";
        int bracketId = proc.find(bracket);
        string name = proc.substr(procId, bracketId - procId);
        names.push_back(Utils::trim(name));
    }
    return names;
}

string Parser:: extractProcName(string procedure) {
    string keyword = Keywords::PROCEDURE;

    int procId = procedure.find(keyword);
    procId = procId + keyword.length();
    string bracket = "{";
    int bracketId = procedure.find(bracket);
    string name = procedure.substr(procId, bracketId - procId);
    return Utils::trim(name);
}

vector<string> Parser:: extractStatements(string procedure) {
    vector<string> statements;
    string openBracket = Keywords::OPEN_EGYPTIAN;
    string closeBracket = Keywords::CLOSE_EGYPTIAN;
    string line;

    int openIdx = procedure.find(openBracket);
    int closeIdx = procedure.find(closeBracket);

    string allStatements = procedure.substr(openIdx + 1, closeIdx - openIdx - 1);

    stringstream s (Utils::trim(allStatements));

    while (getline(s, line, ';')) {
        line = Utils::trim(line);
        statements.push_back(line);
    }

    return statements;
}

shared_ptr<Statement> Parser::parseStatement(string statement, shared_ptr<Procedure> procedureNode) {
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