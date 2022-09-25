using namespace std;

#include <string>
#include <vector>
#include <unordered_map>

#include <ctype.h>

#include "./Validators/ValidatorUtils.h"
#include "QueryTokenizer.h"
#include "QPS.h"
#include <algorithm>


unordered_map<TokenType, SpecificClause> tokenToClauseMap = {
    {TokenType::USES, SpecificClause::USE},
    {TokenType::MODIFIES, SpecificClause::MODIFIES},
    {TokenType::CALL, SpecificClause::CALL},
    {TokenType::WHILE, SpecificClause::WHILE},
    {TokenType::IF, SpecificClause::IF }
};

QueryTokenizer::QueryTokenizer(string queryString) : query(queryString) { }

void QueryTokenizer::resetQueryString(string queryString) {
    query = queryString;
}

vector<PqlToken> QueryTokenizer::Tokenize() {
    if (query.empty()) {
        throw "Invalid Query Syntax :: Query Length is zero.";
    }

    Split();
    ConvertIntoTokens();

    return tokens;
}

void QueryTokenizer::Split() {
    string currentString;
    bool selectExists = false;
    bool insideInvertedCommas = false;

    // Optimization of vector
    delimited_query = vector<string>();
    delimited_query.reserve(query.size()); // Length of query shouldn't be THAT big

    for (int i = 0; i < query.size(); i++) {
        // If the character is a blank (whitespace or tab etc)
        if (isspace(query[i])) {
            if (currentString.empty()) {
                continue;
            }
            else if (insideInvertedCommas) {
                currentString += query[i];
            }
            else {
                delimited_query.push_back(currentString);
                currentString = "";
            }
        }
           
        // If the character is a single character symbol, for eg brackets or comma
        else if (stringToTokenMap.find(string{ query[i] }) != stringToTokenMap.end()) {
            if (currentString != "") {
                delimited_query.push_back(currentString);
            }
            delimited_query.push_back(string{ query[i] });
            currentString = "";
        } 

        else {
            if (query[i] == '"') {
                insideInvertedCommas = !insideInvertedCommas;
            }
            currentString += query[i];
            selectExists = selectExists || currentString == "Select";
        }
    }

  
    if (!currentString.empty()) {
        delimited_query.push_back(currentString);
    }

    if (delimited_query.empty()) {
        throw SyntaxError("Query is blank");
    }

    if (!selectExists) {
        throw SyntaxError("Select keyword does not exist");
    }

    delimited_query.shrink_to_fit(); //Shrinking the final capacity (not size) of the vector for space optimizations

    if (!checkIfDesignEntity(delimited_query[0]) && delimited_query[0] != "Select") {
        throw SyntaxError("Query must either start with a declaration or 'Select' keyword");
    }

    if (delimited_query.back() == ";") {
        throw SyntaxError("Query String should not end with a semicolon");
    }
}



void QueryTokenizer::ConvertIntoTokens() {
    // For Optimization
    tokens = vector<PqlToken>();
    tokens.reserve(delimited_query.size() + 1); // Additional 1 for `declaration end`
    
    int index = 0; // index of delimited_query that we are looping through

    TokenizeDeclarations(index);
    TokenizeSelectAndClauses(index);
}


// Tokenization of Declarations
void QueryTokenizer::TokenizeDeclarations(int& i) {
    bool isCurrentStringSynonym = false; // If not, it must be a keyword TokenType such as a design entity / select etc.
    
    while (i < delimited_query.size()) {
        TokenType currentToken = TokenType::SYNONYM;

        if (delimited_query[i] == "Select" && (i == 0 || tokens.back().type == TokenType::SEMICOLON)) {
            tokens.push_back(PqlToken(TokenType::DECLARATION_END, ""));
            break;
        }

        if (!isCurrentStringSynonym && (checkIfDesignEntity(delimited_query[i]))) {
            currentToken = stringToTokenMap[delimited_query[i]];
            isCurrentStringSynonym = true;
        }
        
        else if (delimited_query[i] == ";") {
            currentToken = TokenType::SEMICOLON;
            isCurrentStringSynonym = false;
        }

        else if (delimited_query[i] == ",") {
            currentToken = TokenType::COMMA;
        }

        else {
            // Similarly, this should be a synonym after a design entity, but we pass it whatever fits
            currentToken = checkDeclarationTokenType(delimited_query[i]);
        }

        tokens.push_back(PqlToken(currentToken, delimited_query[i]));
        i += 1;
    }
}

// Add 'AND' logic for Such That and Pattern Clause

void QueryTokenizer::TokenizeSelectAndClauses(int& i) {
    TokenizeState currentState = TokenizeState::FINDING_KEYWORDS;
    SpecificClause clauseType = SpecificClause::NONE;
    int clauseCounter = 1;

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
                // If this token is not where it is supposed to be, it will throw an error anyway
                currentToken = TokenType::UNKNOWN;
            }
        }

        // TODO: add tuple of elements logic
        else if (currentState == TokenizeState::SELECT) {
            // If it is a symbol like semicolon, or comma
            // Validator will throw an error here
            if (stringToTokenMap.find(delimited_query[i]) != stringToTokenMap.end() && !checkIfSynonym(delimited_query[i])) {
                currentToken = stringToTokenMap[delimited_query[i]];
            }
            else {
                // If it is not a synonym, checkTokenType will assign whatever token the string is, and validator will catch this error
                currentToken = checkSelectTokenType(delimited_query[i]);
            }
            currentState = TokenizeState::FINDING_KEYWORDS; 
        }

        // TODO: "Add AND logic for advanced SPA"
        else {
            if (currentState == TokenizeState::SUCH_THAT) {
                if (clauseCounter == suchThatClauseFirstArgIndex || 
                    clauseCounter == suchThatClauseSecondArgIndex) {
                    currentToken = checkSuchThatTokenType(delimited_query[i], clauseType, clauseCounter);
                }
                else {
                    if (stringToTokenMap.find(delimited_query[i]) != stringToTokenMap.end()) {
                        currentToken = stringToTokenMap[delimited_query[i]];

                        if (clauseCounter == suchThatClauseTypeIndex) {
                            clauseType = tokenToClauseMap[currentToken];
                        }
                    }
                }
            }
            // CurrentState == PATTERN
            else {
                if (clauseCounter % 2 != 0) {
                    currentToken = checkPatternTokenType(delimited_query[i], clauseCounter);
                }
                else {
                    if (stringToTokenMap.find(delimited_query[i]) != stringToTokenMap.end()) {
                        currentToken = stringToTokenMap[delimited_query[i]];
                    }
                }
            }

            clauseCounter += 1;
            
            // If we finish a clause, we reset our clause counter and state
            if (currentToken == TokenType::CLOSED_BRACKET) {
                currentState = TokenizeState::FINDING_KEYWORDS;
                clauseType = SpecificClause::NONE;
                clauseCounter = 1;
            }
        }

        tokens.push_back(PqlToken(currentToken, delimited_query[i]));
        i += 1;
    }
}


// Helper Methods that check of token type of different tokenize states

inline TokenType checkDeclarationTokenType(const string& s) {
    return checkIfSynonym(s) ? TokenType::SYNONYM : TokenType::UNKNOWN;
}

inline TokenType checkSelectTokenType(const string& s) {
    return s == "BOOLEAN" ? TokenType::BOOLEAN
         : checkIfSynonym(s) ? TokenType::SYNONYM
         : TokenType::UNKNOWN;
}

TokenType checkSuchThatTokenType(const string& s, const SpecificClause& clauseType, const int& argNum) {
    switch (clauseType) {
        case SpecificClause::USE:
        case SpecificClause::MODIFIES:
            return argNum == suchThatClauseFirstArgIndex ? getStmtRefToken(s) : getEntRefToken(s);
        case SpecificClause::CALL:
            return getEntRefToken(s);
        default:
            return getStmtRefToken(s);
    }
}

TokenType checkPatternTokenType(const string& s, const int& argNum) {
    switch (argNum) {
        case patternClauseFirstArgIndex:
            return checkIfSynonym(s) ? TokenType::SYNONYM : TokenType::UNKNOWN;
        case patternClauseSecondArgIndex:
            return getEntRefToken(s);
        case patternClauseThirdArgIndex:
            return getExpressionSpec(s);
        default:
            return TokenType::UNKNOWN;
    }
}

// Helper Methods that checks lexical tokens

inline bool checkIfSynonym(const string& s) {
    return isalpha(s[0]) && all_of(s.begin(), s.end(), [](char c) { return isdigit(c) || isalpha(c); });
}

inline bool checkIfInteger(const string& s) {
    if (s.size() == 1) {
        return s == "0" || (isdigit(s[0]) && s != "0");
    }

    return s[0] != '0' && all_of(s.begin(), s.end(), [](char c) { return isdigit(c); });
}

inline TokenType getStmtRefToken(const string& s) {
    if (s == "_") {
        return TokenType::WILDCARD;
    }
    if (checkIfSynonym(s)) {
        return TokenType::SYNONYM;
    }
    if (checkIfInteger(s)) {
        return TokenType::STATEMENT_NUM;
    }

    return TokenType::UNKNOWN;
}

inline TokenType getEntRefToken(const string& s) {
    if (s == "_") {
        return TokenType::WILDCARD;
    }
    if (checkIfSynonym(s)) {
        return TokenType::SYNONYM;
    }
    if (checkIfString(s) && checkIfSynonym(s.substr(1, s.size() - 2))) {
        return TokenType::STRING;
    }

    return TokenType::UNKNOWN;
}

inline TokenType getExpressionSpec(const string& s) {
    if (s == "_") {
        return TokenType::WILDCARD;
    }
    if (checkIfString(s)) {
        return TokenType::STRING;
    }
    if (checkIfWildCardString(s)) {
        return TokenType::WILDCARD_STRING;
    }

    return TokenType::UNKNOWN;
}

inline bool checkIfWildCardString(const string& s) {
    return s[0] == '_' && s[s.size() - 1] == '_' && checkIfString(s.substr(1, s.size() - 2));
}

inline bool checkIfString(const string& s) {
    return s.size() > 2 && s[0] == '"' && s[s.size() - 1] == '"';
}

inline bool checkIfDesignEntity(const string& s) {
    return s == "stmt" || s == "read" || s == "print" || s == "call" || s == "while" || s == "if" || s == "assign" || s == "variable" || s == "constant" || s == "procedure";
}