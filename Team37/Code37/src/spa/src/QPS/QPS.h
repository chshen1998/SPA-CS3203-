using namespace std;

#ifndef TEAM37_QPS_H
#define TEAM37_QPS_H

#include <string>
#include <vector>
#include <unordered_map>
#include <set>
#include <list>


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
};

/*
 * PatternClause has 2 synonyms, the left synonym appears on the LHS of the assignment while the right synonym
 * appears on the RHS of the assignment.
 *
 * These synonyms must be variable type.
 */
struct Clause
{
    string left;
    string right;
};

/*
 * ParsedQueries are created after extracting the components from the query.
 */
struct PqlQuery {
    unordered_map<string, TokenType> declarations;
    string select;
    Clause patternClause;
    Clause suchThatClause;
};


extern unordered_map<string, TokenType> stringToTokenMap;
extern set<TokenType> validDeclarations;
extern set<TokenType> validSuchThatClauses;
extern set<TokenType> validPatternParameters;

class QPS {
public:
    void evaluate(string query, list<string> &results);
};

// These are placeholder methods, delete after integration with PKB
vector<PqlToken> mockTokenize();

#endif //TEAM37_QPS_H

