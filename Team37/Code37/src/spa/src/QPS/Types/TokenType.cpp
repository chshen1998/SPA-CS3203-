using namespace std;

#include <string>
#include <unordered_map>

#include "TokenType.h"

unordered_map<string, TokenType> stringToTokenMap = {
        {"variable", TokenType::VARIABLE},
        {"constant", TokenType::CONSTANT},
        {"assign", TokenType::ASSIGN},
        {"procedure", TokenType::PROCEDURE},
        {"stmt", TokenType::STATEMENT},
        {"while", TokenType::WHILE},
        {"if", TokenType::IF},
        {"else", TokenType::ELSE},
        {"print", TokenType::PRINT},
        {"call", TokenType::CALL},
        {"read", TokenType::READ},

        {"Select", TokenType::SELECT},
        {"pattern", TokenType::PATTERN},
        {"Uses", TokenType::USES},
        {"Modifies", TokenType::MODIFIES},
        {"Follows", TokenType::FOLLOWS},
        {"Follows*", TokenType::FOLLOWS_A},
        {"Parent", TokenType::PARENT},
        {"Parent*", TokenType::PARENT_A},

        {"such", TokenType::SUCH},
        {"that", TokenType::THAT},

        {";", TokenType::SEMICOLON},
        {",", TokenType::COMMA},
        {"(", TokenType::OPEN_BRACKET},
        {")", TokenType::CLOSED_BRACKET},
};