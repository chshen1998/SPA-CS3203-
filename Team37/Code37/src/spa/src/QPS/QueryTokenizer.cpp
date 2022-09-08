using namespace std;

#include <string>
#include <vector>
#include <unordered_map>

#include <ctype.h>

#include "QueryTokenizer.h"
#include "QPS.h"
#include <algorithm>


QueryTokenizer::QueryTokenizer(string queryString) {
    query = queryString;
}

void QueryTokenizer::resetQueryString(string queryString) {
    query = queryString;
    delimited_query = vector<string>();
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

    delimited_query = vector<string>();
    delimited_query.reserve(query.size());

    for (int i = 0; i < query.size(); i++) {

        // If the character is a blank (whitespace or tab etc)
        if (isspace(query[i])) {
            if (currentString.size() == 0) {
                continue;
            }
            else {
                delimited_query.push_back(currentString);
                currentString = "";
            }
        }
           
        // If the character is a symbol
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

    if (!selectExists) {
        throw "Invalid Query Syntax :: Select keyword does not exist.";
    }

    if (currentString.size() > 0) {
        delimited_query.push_back(currentString);
    }

    delimited_query.shrink_to_fit();

    if (delimited_query.size() == 0) {
        throw "Invalid Query Syntax :: Query is blank.";
    }


    if (delimited_query.back() == ";") {
        throw "Invalid Query Syntax :: Query String should not end with a semicolon.";
    }


}



void QueryTokenizer::ConvertIntoTokens() {
    tokens = vector<PqlToken>();
    tokens.reserve(delimited_query.size());

    bool isSynonym = false;

    for (string& element : delimited_query) {
        if (stringToTokenMap.find(element) != stringToTokenMap.end()) {
            TokenType currentTokenType = stringToTokenMap[element];
            tokens.push_back(PqlToken(currentTokenType, element));
          
            
        } 
        else if (checkIfSynonym(element)) {
            tokens.push_back(PqlToken(TokenType::SYNONYM, element));
        }
        else {
            throw "Invalid Query syntax :: Token does not exist.";
        }
    }
}


// Helper Methods for Query Tokenizer
bool checkIfSynonym(string s) {
    return isalpha(s[0]) 
        && all_of(s.begin(), s.end(), 
            [](char c) { return isdigit(c) || isalpha(c); });
}

bool checkIfInteger(string s) {
    if (s.size() == 1) {
        return s == "0";
    }

    return s[0] != '0' && all_of(s.begin(), s.end(), [](char c) { return isdigit(c); });
}
