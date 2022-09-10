#include <vector>
#include <sstream>

using namespace std;

#include "ParserVisitor.h"
#include "AST/SourceCode.h"
#include "SP.h"
#include "Utils.h"
#include "Tokenizer.h"

/**
 * Converts a text file to a string for easier parsing.
 * @param filepath filepath of sourcecode.
 * @return text file as a string.
 */
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

/**
 * Extracts each procedure as a string.
 * @param srcCode source code as a string
 * @param procedures vector to store procedures as strings.
 * @return vector containing procedures as strings
 */
vector<string> SP:: extractProcedures(string srcCode, vector<string> procedures) {
    string keyword = "procedure";

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

/**
 * Extracts the names of each procedure stored in a vector.
 * @param procedures Vector storing the procedures.
 * @return names of each procedure.
 */
vector<string> SP:: extractProcNames(vector<string> procedures) {
    string keyword = "procedure";
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

/**
 * Extracts statements of a procedure as strings.
 * @param procedure
 * @return statements as strings.
 */
vector<string> SP:: extractStatements(string procedure) {
    vector<string> statements;
    string openBracket = "{";
    string closeBracket = "}";
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