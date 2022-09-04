#ifndef TEAM37_PARSER_H
#define TEAM37_PARSER_H

#include <string>
#include <vector>

class Parser {
    int Parse();

    vector<string> AllVariables();
    vector<string> AllConstants();
    vector<string> AllProcedures();
    vector<string> AllStatements();
    vector<PqlToken> mockTokenize();
};


#endif //TEAM37_PARSER_H

using namespace std;

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
    PARENT_T,
    FOLLOWS,
    FOLLOWS_T,

    SUCH,
    THAT,

    SEMICOLON,
    COMMA,
    OPEN_PARENTHESIS,
    CLOSED_PARENTHESIS,

    WHITESPACE,
    END
};

struct PqlToken {
    TokenType type;
    string value;

    PqlToken() : type{ TokenType::STRING }, value{ "" } {}
    PqlToken(TokenType tokenType, string value = "")
            : type{ tokenType }, value{ value } {}
};

struct ParsedQuery {
    unordered_map<string, TokenType> declarations = {};
    string select;
    string pattern;
    string suchThatClause;
};

unordered_map<string, TokenType> stringTokenMap;
unordered_set<TokenType> validDeclarations;
unordered_set<TokenType> validSuchThatClauses;


