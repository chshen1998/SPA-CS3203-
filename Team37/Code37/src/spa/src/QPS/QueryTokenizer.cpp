using namespace std;

#include <string>
#include <vector>
#include <unordered_map>

#include <ctype.h>

#include "QueryTokenizer.h"
#include "QPS.h"
#include <algorithm>


int suchThatClauseTypeIndex = 2;
int suchThatClauseFirstArgIndex = 4;
int suchThatClauseSecondArgIndex = 6;

int patternClauseFirstArgIndex = 1;
int patternClauseSecondArgIndex = 3;
int patternClauseThirdArgIndex = 5;

unordered_map<TokenType, SpecificClause> tokenToClauseMap = {
    {TokenType::USES, SpecificClause::USE},
    {TokenType::FOLLOWS, SpecificClause::FOLLOWS},
    {TokenType::FOLLOWS_A, SpecificClause::FOLLOWS},
    {TokenType::PARENT, SpecificClause::PARENT},
    {TokenType::PARENT_A, SpecificClause::PARENT},
    {TokenType::MODIFIES, SpecificClause::MODIFIES},
    {TokenType::CALL, SpecificClause::CALL},
    {TokenType::WHILE, SpecificClause::WHILE},
    {TokenType::IF, SpecificClause::IF }
};

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
    bool selectExists = false;
    bool inString = false;

    // Optimization of vector
    delimited_query = vector<string>();
    delimited_query.reserve(query.size()); // Length of query shouldn't be THAT big

    for (int i = 0; i < query.size(); i++) {
        // If the character is a blank (whitespace or tab etc)
        if (isspace(query[i])) {
            if (currentString.size() == 0) {
                continue;
            }
           // TODO: Confirm if we want to keep the spaces for pattern arguments with ""
            else if (inString) {
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
                inString = !inString;
            }
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
    tokens.reserve(delimited_query.size() + 1); // Additional 2 for `declaration end`
    
    int index = 0; // index of delimited_query that we are looping through

    TokenizeBeforeSelect(index);
    TokenizeAfterSelect(index);
}


// Tokenization of Declarations
void QueryTokenizer::TokenizeBeforeSelect(int& i) {
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
            currentToken = checkTokenType(delimited_query[i], TokenizeState::DECLARATION, SpecificClause::NONE, ClauseArgNumber::NONE);
        }

        tokens.push_back(PqlToken(currentToken, delimited_query[i]));
        i += 1;
    }
}


void QueryTokenizer::TokenizeAfterSelect(int& i) {
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
                currentToken = checkTokenType(delimited_query[i], currentState, clauseType, ClauseArgNumber::NONE);
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
                currentToken = checkTokenType(delimited_query[i], TokenizeState::SELECT, SpecificClause::NONE, ClauseArgNumber::NONE);
            }
            currentState = TokenizeState::FINDING_KEYWORDS; 
        }

        else {

            if (currentState == TokenizeState::SUCH_THAT) {
                // This should handle Such / that / relRef keyword / openbracket
                // If we catch any other keyword such as 'Select' or 'semicolon', validator will throw an error anyway
                if (clauseCounter == suchThatClauseFirstArgIndex) {
                    currentToken = checkTokenType(delimited_query[i], currentState, clauseType,  ClauseArgNumber::FIRST);
                }
                else if (clauseCounter == suchThatClauseSecondArgIndex) {
                    currentToken = checkTokenType(delimited_query[i], currentState, clauseType , ClauseArgNumber::SECOND);
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

            else {
                // [pattern, token, (, token, comma, token, )
                 // This should handle Such / that / relRef keyword / openbracket
                 // If we catch any other keyword such as 'Select' or 'semicolon', validator will throw an error anyway

                if (clauseCounter == patternClauseFirstArgIndex) {
                    currentToken = checkTokenType(delimited_query[i], currentState, clauseType, ClauseArgNumber::FIRST);
                }

                else if (clauseCounter == patternClauseSecondArgIndex) {
                    currentToken = checkTokenType(delimited_query[i],  currentState, clauseType, ClauseArgNumber::SECOND);
                }

                else if (clauseCounter == patternClauseThirdArgIndex) {
                    currentToken = checkTokenType(delimited_query[i], currentState, clauseType, ClauseArgNumber::THIRD);
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



// Helper Methods for Query Tokenizer
TokenType checkTokenType(const string& s, const TokenizeState& state, const SpecificClause& type, const ClauseArgNumber& argnum) {
    // Declarations
    if (state == TokenizeState::DECLARATION && checkIfSynonym(s)) {
        return TokenType::SYNONYM;
    }
    
    // Select Keyword (Creating a separate if statement from declarations because there will be additional logic for select in advanced SPA)
    if (state == TokenizeState::SELECT && checkIfSynonym(s)) {
        return TokenType::SYNONYM;
    }
    
    // Such that Clauase
    if (state == TokenizeState::SUCH_THAT) {
        if (type == SpecificClause::USE || type == SpecificClause::MODIFIES) {
            if (argnum == ClauseArgNumber::FIRST && checkIfStmtRef(s)) {
                return getStmtRefToken(s);
            }
            if (checkIfEntRef(s)) {
                return getEntRefToken(s);
            }
        }
        else if (type == SpecificClause::CALL && checkIfEntRef(s)) {
            return getEntRefToken(s);
        }
        else {
            if (checkIfStmtRef(s)) {
                return getStmtRefToken(s);
            }
        }
    }

    // Pattern Clauses
    if (state == TokenizeState::PATTERN) {

        // Check whether string is a synonym after the 'pattern' keyword
        if (argnum == ClauseArgNumber::FIRST && checkIfSynonym(s)) {
            return TokenType::SYNONYM;
        }

        // Check whether it is entRef
        if (argnum == ClauseArgNumber::SECOND && checkIfEntRef(s)) {
            return getEntRefToken(s);
        }

        // check expression if assign pattern
        if (argnum == ClauseArgNumber::THIRD && checkIfExpressionSpec(s)) {
            return getExpressionSpec(s);
        }
    }
    
    // If our token is not valid (if it is valid it should pass one of the above cases)
    if (checkIfSynonym(s)) {
        return TokenType::SYNONYM;
    }

    else if (checkIfInteger(s)) {
        return TokenType::NUMBER;
    }

    else {
        return TokenType::UNKNOWN;
    }
}



// Check Token Methods
inline bool checkIfSynonym(const string& s) {
    return isalpha(s[0]) 
        && all_of(s.begin(), s.end(), 
            [](char c) { return isdigit(c) || isalpha(c); });
}

inline bool checkIfInteger(const string& s) {
    if (s.size() == 1) {
        return s == "0" || (isdigit(s[0]) && s != "0");
    }

    return s[0] != '0' && all_of(s.begin(), s.end(), [](char c) { return isdigit(c); });
}

inline bool checkIfStmtRef(const string& s) {
    return s == "_" || checkIfSynonym(s) || checkIfInteger(s);
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
}

inline bool checkIfEntRef(const string& s) {
    return s == "_" || checkIfSynonym(s) || checkIfString(s);
}

inline TokenType getEntRefToken(const string& s) {
    if (s == "_") {
        return TokenType::WILDCARD;
    }
    if (checkIfSynonym(s)) {
        return TokenType::SYNONYM;
    }
    if (checkIfString(s)) {
        return TokenType::STRING;
    }

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
}

inline bool checkIfExpressionSpec(const string& s) {
    return s == "_" || checkIfString(s) || checkIfWildCardString(s);
}

inline bool checkIfWildCardString(const string& s) {
    // TODO: decide if we want to tokenize the assign arguments for easier parsing later
    return s[0] == '_' && s[s.size() - 1] == '_' && checkIfString(s.substr(1, s.size() - 2));
}

inline bool checkIfString(const string& s) {
    if (s.size() < 3) {
        return false;
    }
    return s[0] == '"' && s[s.size() - 1] == '"';
    // Add in this when we implement second argument of pattern clause
    //checkIfSynonym(s.substr(1, s.size() - 2));
}

inline bool checkIfDesignEntity(const string& s) {
    return s == "stmt" || s == "read" || s == "print" || s == "call" || s == "while" || s == "if" || s == "assign" || s == "variable" || s == "constant" || s == "procedure";
}