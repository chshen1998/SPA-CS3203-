#include <vector>
#include <sstream>
#include <fstream>

using namespace std;

#include "AST/SourceCode.h"
#include "SP.h"
#include "Utils.h"
#include "Tokenizer.h"
#include "Keywords.h"

string SP:: fileToString(string filepath) {
    string s;
    string sTotal;

    ifstream in;
    in.open(filepath);

    if (in.is_open()) {
        while(!in.eof()) {
            getline(in, s);
            sTotal += s + "\n";
        }
        in.close();
    } else {
        cout << "Unable to open file." << endl;
    }

    return sTotal;
}

vector<string> SP:: extractProcedures(string srcCode, vector<string> procedures) {
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

vector<string> SP:: extractProcNames(vector<string> procedures) {
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

vector<string> SP:: extractStatements(string procedure) {
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

shared_ptr<SourceCode> SP::parse(string filename) {
    string code = fileToString(filename);
    vector<string> procedures;
    procedures = extractProcedures(code, procedures);
    vector<string> procNames = extractProcNames(procedures);

    vector<vector<string> > statementLists;
    for (auto p: procedures) {
        vector<string> stmts = extractStatements(p);
        statementLists.push_back(stmts);
    }

    shared_ptr<SourceCode> sourceCode = make_shared<SourceCode>(filename);
    sourceCode = Tokenizer::tokenize(sourceCode, procNames, statementLists);
    return sourceCode;
}