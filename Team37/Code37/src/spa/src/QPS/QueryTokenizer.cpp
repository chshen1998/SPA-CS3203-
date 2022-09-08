using namespace std;

#include <string>
#include <vector>
#include <unordered_map>

#include <ctype.h>

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


void QueryTokenizer::Split() {
    string currentString;
    bool selectExists{};
    bool addToList{};

    for (int i = 0; i < query.size(); i++) {

        // If the character is a blank (whitespace or tab etc)
        if (isblank(query[i])) {
            if (currentString.size() == 0) {
                continue;
            }
            else {
                delimited_query.push_back(currentString);
                currentString = "";
            }
        }

        // If the character is a symbol (whitespace or tab etc)
        else if (stringToTokenMap.find(string{ query[i] }) != stringToTokenMap.end()) {
            delimited_query.push_back(currentString);
            delimited_query.push_back(string{ query[i] });
            currentString = "";
        }

        else {
            currentString += query[i];
            selectExists = selectExists || currentString == "Select";
        }
    }

    if (currentString.size() > 0) {
        delimited_query.push_back(currentString);
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