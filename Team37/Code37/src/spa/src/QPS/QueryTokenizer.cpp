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
    delimited_query = vector<string>();
    tokens = vector<PqlToken>();
}

void QueryTokenizer::resetQueryString(string queryString) {
    query = queryString;
    delimited_query = vector<string>();
    tokens = vector<PqlToken>();
}


vector<PqlToken> QueryTokenizer::Tokenize() {
    Split();
    ConvertIntoTokens();

    return tokens;
}


void QueryTokenizer::Split() {
    string currentString;
    bool selectExists {};
    bool addToList {};

    if (query.size() == 0) {
        throw "Invalid Query Syntax :: Query Length is zero.";
    }

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

    if (currentString.size() > 0) {
        delimited_query.push_back(currentString);
    }

    if (delimited_query.size() == 0) {
        throw "Invalid Query Syntax :: Query is blank.";
    }

    if (!selectExists) {
        throw "Invalid Query Syntax :: Select keyword does not exist.";
    }

    if (delimited_query.back() == ";") {
        throw "Invalid Query Syntax :: Query String should not end with a semicolon.";
    }
}



void QueryTokenizer::ConvertIntoTokens() {
    
    for (string& element : delimited_query) {
        if (stringToTokenMap.find(element) != stringToTokenMap.end()) {
            tokens.push_back(PqlToken(stringToTokenMap[element], element));
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
