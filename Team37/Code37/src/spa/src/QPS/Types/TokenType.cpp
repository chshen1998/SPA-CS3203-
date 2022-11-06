using namespace std;

#include <string>
#include <unordered_map>

#include "TokenType.h"

unordered_map<string, TokenType> stringToTokenMap = {
    { "variable", TokenType::VARIABLE },
    { "constant", TokenType::CONSTANT },
    { "assign", TokenType::ASSIGN },
    { "procedure", TokenType::PROCEDURE },
    { "stmt", TokenType::STATEMENT },
    { "while", TokenType::WHILE },
    { "if", TokenType::IF },
    { "else", TokenType::ELSE },
    { "print", TokenType::PRINT },
    { "call", TokenType::CALL },
    { "read", TokenType::READ },

    { "Select", TokenType::SELECT },
    { "BOOLEAN", TokenType::BOOLEAN },
    { "pattern", TokenType::PATTERN },
    { "with", TokenType::WITH },
    { "Uses", TokenType::USES },
    { "Modifies", TokenType::MODIFIES },
    { "Follows", TokenType::FOLLOWS },
    { "Follows*", TokenType::FOLLOWS_A },
    { "Parent", TokenType::PARENT },
    { "Parent*", TokenType::PARENT_A },
    { "Calls", TokenType::CALLS },
    { "Calls*", TokenType::CALLS_A },
    { "Next", TokenType::NEXT },
    { "Next*", TokenType::NEXT_A },
    { "Affects", TokenType::AFFECTS },
    { "Affects*", TokenType::AFFECTS_A },

    { "procName", TokenType::PROCNAME },
    { "varName", TokenType::VARNAME },
    { "value", TokenType::VALUE },
    { "stmt#", TokenType::STMTLINE },

    { "such", TokenType::SUCH },
    { "that", TokenType::THAT },
    { "with", TokenType::WITH },

    { ";", TokenType::SEMICOLON },
    { ",", TokenType::COMMA },
    { ".", TokenType::DOT },
    { "=", TokenType::EQUAL },
    { "(", TokenType::OPEN_BRACKET },
    { ")", TokenType::CLOSED_BRACKET },
    { "<", TokenType::OPEN_ARROW },
    { ">", TokenType::CLOSED_ARROW },
};