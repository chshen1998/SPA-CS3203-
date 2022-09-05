using namespace std;

#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>

#include "QueryTokenizer.h"
#include "QPS.h"

QueryTokenizer::QueryTokenizer(string queryString) {
    query = queryString;
    delimited_query = vector<string>();
    tokens = vector<PqlToken>();
}

vector<PqlToken> QueryTokenizer::Tokenize() {
    if (query.size() == 0) {
        throw "Invalid Query Syntax :: Query Length is zero.";
    }

    Split();
    ConvertIntoTokens();

    return tokens;
}

/*
TODO: To be added and revamped.
Currently hard-coded for the demo.
*/
void QueryTokenizer::Split() {
    TokenType currentToken;
    string currentString;
    bool selectExists;

    for (int i = 0; i < query.size(); i++) {
        if (query[i] == '') {

        }

        currentString += query[i];


    }

    if (!selectExists) {
        throw "Invalid Query Syntax :: Select statement does not exist.";
    }

}

void QueryTokenizer::ConvertIntoTokens() {
    for (string& element : delimited_query) {
        if (stringToTokenMap.find(element) != stringToTokenMap.end()) {
            tokens.push_back(PqlToken(stringToTokenMap[element], element));
        }
        else {
            throw "Invalid Query syntax :: Token does not exist.";
        }
    }
}
