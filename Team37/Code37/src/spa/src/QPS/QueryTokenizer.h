using namespace std;

#ifndef TEAM37_QUERYTOKENIZER_H
#define TEAM37_QUERYTOKENIZER_H

#include "QPS.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <set>

class QueryTokenizer {
public:
    vector<PqlToken> tokens;
    vector<string> delimited_query;
    string query;

    QueryTokenizer(string query);

    vector<PqlToken> Tokenize();
    void resetQueryString(string query);

private:
    void Split();
    void ConvertIntoTokens();
};

bool checkIfSynonym(string s);
bool checkIfInteger(string s);

#endif //TEAM37_QPS_H
