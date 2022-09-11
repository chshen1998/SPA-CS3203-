using namespace std;

#ifndef TEAM37_QUERYTOKENIZER_H
#define TEAM37_QUERYTOKENIZER_H

#include "QPS.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <set>

enum class TokenizeState {
    FINDING_KEYWORDS,
    SELECT,
    SUCH_THAT,
    PATTERN
};



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
    int TokenizeBeforeSelect(int i);
    void TokenizeAfterSelect(int i);
};

TokenType checkTokenType(string s);
bool checkIfSynonym(string s);
bool checkIfInteger(string s);
bool checkIfStmtRef(string s);
bool checkIfEntRef(string s);
bool checkIfDesignEntity(string s);


#endif //TEAM37_QPS_H
