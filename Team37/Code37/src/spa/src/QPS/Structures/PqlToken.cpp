using namespace std;

#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>

#include "../Types/TokenType.h"
#include "PqlToken.h"

std::ostream& operator<< (std::ostream& os, const PqlToken& token) {
    string typeString = "Unknown";
    switch (token.type) {
    case(TokenType::VARIABLE): typeString = "variable"; break;
    case(TokenType::CONSTANT):typeString = "constant"; break;
    case(TokenType::ASSIGN): typeString = "assign"; break;
    case(TokenType::PROCEDURE): typeString = "procedure"; break;
    case(TokenType::STATEMENT): typeString = "statement"; break;
    case(TokenType::WHILE): typeString = "while"; break;
    case(TokenType::IF): typeString = "if"; break;
    case(TokenType::ELSE): typeString = "else"; break;
    case(TokenType::PRINT): typeString = "print"; break;
    case(TokenType::CALL): typeString = "call"; break;
    case(TokenType::SYNONYM): typeString = "synonym"; break;
    case(TokenType::NUMBER): typeString = "number"; break;
    case(TokenType::STATEMENT_NUM): typeString = "statement_num"; break;
    case(TokenType::WILDCARD): typeString = "wildcard"; break;
    case(TokenType::WILDCARD_STRING): typeString = "wildcard_string"; break;
    case(TokenType::STRING): typeString = "string"; break;
    case(TokenType::SELECT): typeString = "select"; break;
    case(TokenType::PATTERN): typeString = "pattern"; break;
    case(TokenType::USES): typeString = "uses"; break;
    case(TokenType::MODIFIES): typeString = "modifies"; break;
    case(TokenType::PARENT): typeString = "parent"; break;
    case(TokenType::PARENT_A): typeString = "parent_a"; break;
    case(TokenType::FOLLOWS): typeString = "follows"; break;
    case(TokenType::FOLLOWS_A): typeString = "follows_a"; break;
    case(TokenType::SUCH): typeString = "such"; break;
    case(TokenType::THAT): typeString = "that"; break;
    case(TokenType::SEMICOLON): typeString = "semicolon"; break;
    case(TokenType::COMMA): typeString = "comma"; break;
    case(TokenType::OPEN_BRACKET): typeString = "open_bracket"; break;
    case(TokenType::CLOSED_BRACKET): typeString = "closed_bracket"; break;
    case(TokenType::EMPTY): typeString = "empty"; break;
    case(TokenType::DECLARATION_END): typeString = "declaration_end"; break;
    case(TokenType::END): typeString = "end"; break;
    }

    os << "[ TokenType: " + typeString + " , Value : '" + token.value + "' ]" << endl;

    return os;
}