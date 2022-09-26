using namespace std;

#include <string>
#include <unordered_map>

#ifndef TEAM37_TOKENTYPE_H
#define TEAM37_TOKENTYPE_H

/*
 * TokenTypes are used to tokenize query string, each word in the query string should be able to fit in one of the
 * existing token types
 */
enum class TokenType {
    UNKNOWN,

    VARIABLE,
    CONSTANT,
    ASSIGN,
    PROCEDURE,
    STATEMENT,

    WHILE,
    IF,
    ELSE,
    READ,
    PRINT,
    CALL,

    BOOLEAN,
    SYNONYM,
    NUMBER,
    STRING, // Anything with a inverted commas ""
    STATEMENT_NUM,
    WILDCARD,
    WILDCARD_STRING,

    SELECT,
    PATTERN,
    USES,
    USES_P,
    MODIFIES,
    MODIFIES_P,
    PARENT,
    PARENT_A,
    FOLLOWS,
    FOLLOWS_A,

    SUCH,
    THAT,
    AND,

    SEMICOLON,
    COMMA,
    OPEN_BRACKET,
    CLOSED_BRACKET,

    EMPTY,
    DECLARATION_END,
    END
};

/*
 * Hashmap containing the tokenType to pql string value mapping
 */
extern unordered_map<string, TokenType> stringToTokenMap;

#endif 