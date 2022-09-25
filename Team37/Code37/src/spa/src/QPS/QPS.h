using namespace std;

#ifndef TEAM37_QPS_H
#define TEAM37_QPS_H

#include <string>
#include <vector>
#include <unordered_map>
#include <set>
#include <list>
#include <iostream>
#include<memory>

#include "PKB/QueryServicer.h"


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
 * PqlTokens are created after tokenizing an element from query string.
 * PqlToken contains the string value from the query string and the token type.
 */
struct PqlToken {
    TokenType type;
    string value;

    PqlToken(TokenType tokenType, string tokenValue) {
        type = tokenType;
        value = tokenValue;
    }

    bool operator==(const PqlToken& other) const {
        return (other.type == type) && (other.value == value);
    }

public:
    // For debugging when writing unit tests
    friend ostream& operator<< (std::ostream& os, const PqlToken& token);
       
};

enum class ErrorType
{
    SYNTAX_ERROR,
    SEMANTIC_ERROR,
    NONE
};

struct PqlError
{
    ErrorType errorType;
    string message;

    PqlError(ErrorType type, string msg) : errorType(type), message(msg) {}
};


/*
 * PatternClause has 2 synonyms, the left synonym appears on the LHS of the assignment while the right synonym
 * appears on the RHS of the assignment.
 *
 * These synonyms must be variable type.
 */
struct Clause
{
    PqlToken clauseType;
    PqlToken left;
    PqlToken right;

    Clause(PqlToken type, PqlToken l, PqlToken r) : clauseType(type), left(l), right(r) {}

    bool operator==(const Clause& other) const {
        return (other.clauseType == clauseType) && (other.left == left) && (other.right == right);
    }
};

/*
 * ParsedQueries are created after extracting the components from the query.
 */
struct PqlQuery {
    unordered_map<string, TokenType> declarations = {};
    string select;
    vector<Clause> patternClauses;
    vector<Clause> suchThatClauses;
};

extern unordered_map<string, TokenType> stringToTokenMap;

class QPS {
    shared_ptr<QueryServicer> servicer;

public:
    void setQueryServicer(shared_ptr<QueryServicer> s);
    void evaluate(string query, list<string> &results);
};


#endif //TEAM37_QPS_H

