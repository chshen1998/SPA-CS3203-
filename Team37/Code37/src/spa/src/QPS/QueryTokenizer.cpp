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
    Split();
    ConvertIntoTokens();

    return tokens;
}

/*
TODO: To be added and revamped.
Currently hard-coded for the demo.
*/
void QueryTokenizer::Split() {
    istringstream iss(query);
    string current;
}

void QueryTokenizer::ConvertIntoTokens() {
    TokenType current;

    for (string& element : delimited_query) {
        if (stringToTokenMap.find(element) != stringToTokenMap.end()) {
            tokens.push_back(PqlToken(stringToTokenMap[element], element));
        }
        else {
            throw "Invalid Query syntax";
        }
    }
}
