using namespace std;

#ifndef TEAM37_PARSER_H
#define TEAM37_PARSER_H

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>


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

    PqlToken() : type{ TokenType::STRING }, value{ "" } {}
    PqlToken(TokenType tokenType, string value = "")
            : type{ tokenType }, value{ value } {}
};

/*
 * ParsedQueries are created after extracting the components from the query.
 */
struct PqlQuery {
    unordered_map<string, TokenType> declarations = {};
    string select;
    string pattern;
    string suchThatClause;
};

extern unordered_map<string, TokenType> stringTokenMap;
extern unordered_set<TokenType> validDeclarations;
extern unordered_set<TokenType> validSuchThatClauses;

class Parser {
public:
    int Parse();
};

// These are placeholder methods, delete after integration with PKB
vector<string> AllVariables();
vector<string> AllConstants();
vector<string> AllProcedures();
vector<string> AllStatements();
vector<PqlToken> mockTokenize();

#endif //TEAM37_PARSER_H

