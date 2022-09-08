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

    if (!checkIfDesignEntity(delimited_query[0]) && delimited_query[0] != "Select") {
        throw "Invalid Query Syntax :: Query must either start with a declaration or 'Select' keyword.";
    }

    if (delimited_query.back() == ";") {
        throw "Invalid Query Syntax :: Query String should not end with a semicolon.";
    }
}



void QueryTokenizer::ConvertIntoTokens() {
    // For Optimization
    tokens = vector<PqlToken>();
    tokens.reserve(delimited_query.size() + 2); // Additional 2 for `declaration end` and `end`


    int i = 0;
    bool isCurrentStringSynonym = false; // If not, it must be a keyword TokenType such as a design entity / select etc.

    // Tokenization of Declarations
    while (i < delimited_query.size()) {

        // If it does not start with a design entity, then it must be a select statement
        if (i == 0 && !checkIfDesignEntity(delimited_query[i])) {
            break;
        }

        if (delimited_query[i] == "Select" && tokens.back().type == TokenType::SEMICOLON) {
            tokens.push_back(PqlToken(TokenType::DECLARATION_END, ""));
            isCurrentStringSynonym = false;
            break;
        }

        TokenType currentToken = TokenType::SYNONYM;

        if (!isCurrentStringSynonym) {
            if (checkIfDesignEntity(delimited_query[i])) {
                currentToken = stringToTokenMap[delimited_query[i]];
                isCurrentStringSynonym = true;
            }
            else {
                throw "Invalid Query Syntax :: Declaration needs to start with a design entity";
            }
        }

        else {
            if (delimited_query[i] == ";") {
                currentToken = TokenType::SEMICOLON;
                isCurrentStringSynonym = false;
            }

            if (delimited_query[i] == ",") {
                currentToken = TokenType::COMMA;
            }

            if (!checkIfSynonym(delimited_query[i])) {
                throw "Invalid Query Syntax :: Declaration must contain only synonyms";
            }
        }

        tokens.push_back(PqlToken(currentToken, delimited_query[i]));
        i += 1;
    }

    // Tokenization of Select Keyword
    if (delimited_query[i] != "Select") {
        throw "Invalid Query Syntax:: 'Select' keyword must come immediately after Declaration";
    } 

    tokens.push_back(PqlToken(TokenType::SELECT, delimited_query[i]));
    i += 1;

    /*
    UPDATE THIS IN THE FUTURE FOR ADVANCED PQL
    -- Will have BOOLEAN | elem | Tuple(Elem), but for now is just synonym
    */
    if (!checkIfSynonym(delimited_query[i])) {
        throw "Invalid Query Syntax:: 'Select' keyword must come immediately after Declaration";
    }

    tokens.push_back(PqlToken(TokenType::SYNONYM, delimited_query[i]));
    i += 1;


    // Tokenization of  After Select Statement
    // TODO: Check for such as clause and pattern clause
    while (i < delimited_query.size()) {
        i += 1;
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

bool checkIfStmtRef(string s) {
    return s == "_" || checkIfSynonym(s) || checkIfInteger(s);
}

bool checkIfDesignEntity(string s) {
    return s == "stmt" || s == "read" || s == "print" || s == "call" || s == "while" || s == "if" || s == "assign" || s ==
        "variable" || s == "constant" || s == "procedure";
}