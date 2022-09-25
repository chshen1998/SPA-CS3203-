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
    // The default type if our tokenizer is unable to identify the string
    UNKNOWN,

    // Design Entities
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

    // Arguments / return types 
    BOOLEAN,
    SYNONYM,
    NUMBER,
    STRING, // Anything with a inverted commas ""
    STATEMENT_NUM,
    WILDCARD,
    WILDCARD_STRING,

    // Keywords
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
    CALLS,
    CALLS_A,
    NEXT,
    NEXT_A,
    AFFECTS,
    AFFECTS_A,

    // Filter words
    SUCH,
    THAT,
    AND,
    WITH,

    // Attribute Names
    PROCNAME,
    VARNAME,
    VALUE,
    STMTLINE,

    // Symbols
    SEMICOLON,
    COMMA,
    DOT,
    OPEN_BRACKET,
    CLOSED_BRACKET,
    OPEN_ARROW,
    CLOSED_ARROW,

    EMPTY,
    DECLARATION_END,
    END
};

/*
 * Hashmap containing the tokenType to pql string value mapping
 */
extern unordered_map<string, TokenType> stringToTokenMap;

#endif 