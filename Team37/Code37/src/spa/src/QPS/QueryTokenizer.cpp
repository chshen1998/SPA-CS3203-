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

    // Optimization of vector
    delimited_query = vector<string>();
    delimited_query.reserve(query.size()); // Length of query shouldn't be THAT big

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

    delimited_query.shrink_to_fit(); //Shrinking the final capacity (not size) of the vector for space optimizations

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
    int index = 0; // index of delimited_string

    index = TokenizeBeforeSelect(index);
    TokenizeAfterSelect(index);
}


int QueryTokenizer::TokenizeBeforeSelect(int i) {
    bool isCurrentStringSynonym = false; // If not, it must be a keyword TokenType such as a design entity / select etc.
    
    // Tokenization of Declarations
    while (i < delimited_query.size()) {
        TokenType currentToken = TokenType::SYNONYM;

        if (delimited_query[i] == "Select" && (i == 0 || tokens.back().type == TokenType::SEMICOLON)) {
            tokens.push_back(PqlToken(TokenType::DECLARATION_END, ""));
            break;
        }

        if (!isCurrentStringSynonym) {
            if (checkIfDesignEntity(delimited_query[i])) {
                currentToken = stringToTokenMap[delimited_query[i]];
                isCurrentStringSynonym = true;
            }
            else {
                // If it is supposed to be a design entity, but it is not, we add in a token with whatever fits
                // This should throw an error in validator!
                currentToken = checkTokenType(delimited_query[i]);
            }
        }
        else {
            if (delimited_query[i] == ";") {
                currentToken = TokenType::SEMICOLON;
                isCurrentStringSynonym = false;
            }

            else if (delimited_query[i] == ",") {
                currentToken = TokenType::COMMA;
            }

            else {
                // Similarly, this should be a synonym after a design entity, but we pass it whatever fits
                currentToken = checkTokenType(delimited_query[i]);
            }
        }

        tokens.push_back(PqlToken(currentToken, delimited_query[i]));
        i += 1;
    }

    return i;
}


void QueryTokenizer::TokenizeAfterSelect(int i) {
    bool isCurrentStringSynonym = false; // If not, it must be a keyword TokenType such as / select / such that clause etc.
    TokenizeState currentState = TokenizeState::FINDING_KEYWORDS;
    int clauseCounter = 1;

    // Tokenization of Declarations
    while (i < delimited_query.size()) {
        TokenType currentToken = TokenType::SYNONYM;

        if (currentState == TokenizeState::FINDING_KEYWORDS) {
            if (stringToTokenMap.find(delimited_query[i]) != stringToTokenMap.end()) {
                currentToken = stringToTokenMap[delimited_query[i]];

                if (currentToken == TokenType::SELECT) {
                    currentState = TokenizeState::SELECT;
                }
                if (currentToken == TokenType::SUCH) {
                    currentState = TokenizeState::SUCH_THAT;
                }
                if (currentToken == TokenType::PATTERN) {
                    currentState = TokenizeState::PATTERN;
                }
            }
            else {
                currentToken = checkTokenType(delimited_query[i]);
            }
        }

        /*
        * For basic PQL, We only check for ONE synonym after 'Select'
        * TODO: For Advanced PQL, we need to check of elem | tuple etc.
        */
        else if (currentState == TokenizeState::SELECT) {
            // If it is a symbol like semicolon, or comma
            // Validator will throw an error here
            if (stringToTokenMap.find(delimited_query[i]) != stringToTokenMap.end() && !checkIfSynonym(delimited_query[i])) {
                currentToken = stringToTokenMap[delimited_query[i]];
            }
            else {
                // If it is not a synonym, checkTokenType will assign whatever token the string is, and validator will catch this error
                currentToken = checkTokenType(delimited_query[i]);
            }
            currentState = TokenizeState::FINDING_KEYWORDS; 
        }

        else if (currentState == TokenizeState::SUCH_THAT) {
            // This should handle Such / that / relRef keyword / openbracket
            // If we catch any other keyword such as 'Select' or 'semicolon', validator will throw an error anyway
            if (stringToTokenMap.find(delimited_query[i]) != stringToTokenMap.end()) {
                // [such, that, relationshipKeyword, (, token, comma, token, )]
                // checking based on index
                if (clauseCounter <= 4 || clauseCounter == 6 || clauseCounter == 8) {
                    currentToken = stringToTokenMap[delimited_query[i]];
                }
                else {
                    currentToken = checkTokenType(delimited_query[i]);
                }

                // Closed bracket signifies the end of a caluse
                if (currentToken == TokenType::CLOSED_BRACKET) {
                    currentState = TokenizeState::FINDING_KEYWORDS;
                    clauseCounter = 0;
                }
            }
            else {
                // This will ensure that the string is stmt_ref or ent_ref as need be or validator will throw an error
                currentToken = checkTokenType(delimited_query[i]);

                if (clauseCounter <= 4 || clauseCounter == 6 || clauseCounter == 8) {
                    // If we encounter a wrong token, we reset.
                    currentState = TokenizeState::FINDING_KEYWORDS;
                    clauseCounter = 0;
                }
            }

            clauseCounter += 1;
        }

        else {
            // [pattern, token, (, token, comma, token, )
             // This should handle Such / that / relRef keyword / openbracket
             // If we catch any other keyword such as 'Select' or 'semicolon', validator will throw an error anyway
             if (stringToTokenMap.find(delimited_query[i]) != stringToTokenMap.end()) {
                 if (clauseCounter == 2|| clauseCounter == 4 || clauseCounter == 6) {
                     currentToken = stringToTokenMap[delimited_query[i]];
                 }
                 else {
                     currentToken = checkTokenType(delimited_query[i]);
                 }

                 // Closed bracket signifies the end of a caluse
                 if (currentToken == TokenType::CLOSED_BRACKET) {
                     currentState = TokenizeState::FINDING_KEYWORDS;
                     clauseCounter = 0;
                 }
             }
             else {
                 // This will ensure that the string is stmt_ref or ent_ref as need be or validator will throw an error
                 currentToken = checkTokenType(delimited_query[i]);

                 if (clauseCounter <= 2 || clauseCounter == 4 || clauseCounter == 6) {
                     // If we encounter a wrong token, we reset.
                     currentState = TokenizeState::FINDING_KEYWORDS;
                     clauseCounter = 0;
                 }
             }
             clauseCounter += 1;
        }

        tokens.push_back(PqlToken(currentToken, delimited_query[i]));
        i += 1;
    }

    tokens.push_back(PqlToken(TokenType::END, ""));
}



// Helper Methods for Query Tokenizer
TokenType checkTokenType(string s) {
    if (checkIfSynonym(s)) {
        return TokenType::SYNONYM;
    }

    else if (checkIfInteger(s)) {
        return TokenType::NUMBER;
    }

    else if (checkIfEntRef(s)) {
        return TokenType::STRING;
    }

    else if (checkIfStmtRef(s)) {
        return TokenType::STATEMENT;
    }
}


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

bool checkIfEntRef(string s) {
    return s == "_" || checkIfSynonym(s) || s[0] == '"' && s[s.size() - 1] == '"' && checkIfSynonym(s.substr(1, s.size() - 2));
}

bool checkIfDesignEntity(string s) {
    return s == "stmt" || s == "read" || s == "print" || s == "call" || s == "while" || s == "if" || s == "assign" || s ==
        "variable" || s == "constant" || s == "procedure";
}