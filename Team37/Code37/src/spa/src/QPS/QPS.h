using namespace std;

#ifndef TEAM37_QPS_H
#define TEAM37_QPS_H

#include <string>
#include <vector>
#include <unordered_map>
#include <set>


/*
 * TokenTypes are used to tokenize query string, each word in the query string should be able to fit in one of the
 * existing token types
 */
enum class TokenType {
    VARIABLE,
    CONSTANT,
    ASSIGN,
    PROCEDURE,
    STATEMENT,

    WHILE,
    IF,
    ELSE,

    SYNONYM,
    NUMBER,
    STRING,
    STATEMENT_NUM,

    SELECT,
    PATTERN,
    USES,
    MODIFIES,
    PARENT,
    PARENT_A,
    FOLLOWS,
    FOLLOWS_A,

    SUCH,
    THAT,

    SEMICOLON,
    COMMA,
    OPEN_BRACKET,
    CLOSED_BRACKET,

    WHITESPACE,
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
};

enum class ErrorType
{
    SYNTAX_ERROR,
    SEMANTIC_ERROR,
    NONE
};

struct PqlError
{
    ErrorType type;
    string message;

    PqlError(ErrorType type) : type(type) {};
};

/*
 * ParsedQueries are created after extracting the components from the query.
 */
struct PqlQuery {
    unordered_map<string, TokenType> declarations;
    string select;
    string pattern;
    string suchThatClause;
};

extern unordered_map<string, TokenType> stringToTokenMap;
extern set<TokenType> validDeclarations;
extern set<TokenType> validSuchThatClauses;

class QPS {
public:
    string processQuery(string query);
};

// These are placeholder methods, delete after integration with PKB
set<string> AllVariables();
set<string> AllConstants();
set<string> AllProcedures();
set<string> AllStatements();
vector<PqlToken> mockTokenize();

#endif //TEAM37_QPS_H

