using namespace std;

#ifndef TEAM37_TOKENIZER_UTILS_H
#define TEAM37_TOKENIZER_UTILS_H

#include <algorithm>
#include <string>
#include <unordered_map>

#include "../Types/TokenType.h"

enum class ClauseType {
    NONE,

    ATTR_REF,
    TUPLE,
    SINGLE,

    USE,
    MODIFIES,
    CALLS,

    ASSIGN,
    WHILE,
    IF
};

namespace TokenizerUtils {
extern unordered_map<TokenType, ClauseType> tokenTypeToClauseTypeMap;

inline bool checkIfSynonym(const string& s)
{
    return isalpha(s[0]) && all_of(s.begin(), s.end(), [](char c) { return isdigit(c) || isalpha(c); });
}

inline bool checkIfInteger(const string& s)
{
    if (s.size() == 1) {
        return s == "0" || (isdigit(s[0]) && s != "0");
    }

    return s[0] != '0' && all_of(s.begin(), s.end(), [](char c) { return isdigit(c); });
}

inline bool checkIfString(const string& s)
{
    return s.size() > 2 && s[0] == '"' && s[s.size() - 1] == '"';
}

inline bool checkIfWildCardString(const string& s)
{
    return s[0] == '_' && s[s.size() - 1] == '_' && checkIfString(s.substr(1, s.size() - 2));
}

inline bool checkIfDesignEntity(const string& s)
{
    return s == "stmt" || s == "read" || s == "print" || s == "call" || s == "while" || s == "if" || s == "assign" || s == "variable" || s == "constant" || s == "procedure";
}

inline bool checkIfAttrName(const string& s)
{
    return s == "procName" || s == "varName" || s == "value" || s == "stmt#";
}

inline TokenType getStmtRefToken(const string& s)
{
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

inline TokenType getEntRefToken(const string& s)
{
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

inline TokenType getExpressionSpec(const string& s)
{
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

inline bool checkValidSymbol(const string& s)
{
    return stringToTokenMap.find(s) != stringToTokenMap.end() && !checkIfSynonym(s);
}
}
#endif