using namespace std;

#ifndef TEAM37_SP_H
#define TEAM37_SP_H

#include <string>
#include <vector>
#include <AST/TNode.h>

class SP {
public:
    static shared_ptr<TNode> parse(string filename);

    shared_ptr<SourceCode> processSourceCode(string filename);

    string fileToString(string filename);

    vector<string> extractProcedures(string srcCode, vector<string> procedures)

    vector<string> extractProcedureName(vector<string> procedures);

    vector<shared_ptr<Procedure>> extractStatements(ifstream file);

    vector<string> extractProcNames(vector<string> procedures);

    vector<string> extractStatements(vector<string> procedures);
};

#endif