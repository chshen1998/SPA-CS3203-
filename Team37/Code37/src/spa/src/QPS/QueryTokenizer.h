using namespace std;

#ifndef TEAM37_QUERYTOKENIZER_H
#define TEAM37_QUERYTOKENIZER_H

#include "./Structures/PqlError.h"
#include "./Structures/PqlToken.h"
#include "./Structures/PqlQuery.h"
#include "./Types/ErrorType.h"
#include "./Types/TokenType.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <set>

#include "QPS.h"


/*
* Represents the current state when tokenizing our delimited query
* The state corresponds to what kind of keyword/clause we are looking at
*/
enum class TokenizeState {
    FINDING_KEYWORDS, 
    SELECT,
    SUCH_THAT,
    PATTERN,
    WITH
};


class QueryTokenizer {
public:
    vector<PqlToken> tokens;
    vector<string> delimited_query;
    string query;

    QueryTokenizer(string);

    vector<PqlToken> tokenize();
    void resetQueryString(string query);

private:
    void split();
    void convertIntoTokens();

};

#endif //TEAM37_QPS_H
