#include <vector>

using namespace std;

#include "ParserVisitor.h"
#include "AST/SourceCode.h"
#include "SP.h"

string toString(string filename) {
    string s;
    string sTotal;

    ifstream in;
    in.open(filename);

    while(!in.eof()) {
        getline(in, s);
        sTotal += s + "\n";
    }
    in.close();
    return sTotal;
}

vector<string> extractProcedures(string srcCode, vector<string> procedures) {
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

vector<string> extractProcedureName(vector<string> procedures) {

}

vector<shared_ptr<Procedure> > SP:: extractStatements(ifstream file) {

}

shared_ptr<SourceCode> SP:: processSourceCode(string filename) {
    ifstream file (filename);
    if (file.is_open()) {
        string keyword = "procedure";
        while (getline(file, keyword)) {

        }
    }
}

shared_ptr<TNode> SP::parse(string filename) {
    shared_ptr<ParserVisitor> parserVisitor = make_shared<ParserVisitor>();

    SourceCode sourceCode = SourceCode(filename);


//    sourceCode.accept(parserVisitor);
}