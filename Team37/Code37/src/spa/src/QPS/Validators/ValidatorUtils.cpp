using namespace std;

#include <set>
#include <unordered_map>

#include "../Structures/PqlError.h"
#include "../Structures/PqlToken.h"
#include "../Structures/PqlQuery.h"
#include "../Types/ErrorType.h"
#include "../Types/TokenType.h"
#include "ValidatorUtils.h"


SyntaxError::SyntaxError(string msg) : exception()
{
    message = msg;
}

SemanticError::SemanticError(string msg) : exception()
{
    message = msg;
}

/*
 * Set of valid declaration types
 */
set<TokenType> validDesignEntities = {
	TokenType::VARIABLE,
	TokenType::PROCEDURE,
	TokenType::WHILE,
	TokenType::ASSIGN,
	TokenType::STATEMENT,
    TokenType::READ,
    TokenType::PRINT,
    TokenType::CALL,
    TokenType::IF,
    TokenType::CONSTANT
};

/*
 * Set of valid such that clauses
 */
set<TokenType> validRelationshipsRef = {
    TokenType::USES,
    TokenType::USES_P,
    TokenType::MODIFIES,
    TokenType::MODIFIES_P,
    TokenType::PARENT,
    TokenType::PARENT_A,
    TokenType::FOLLOWS,
    TokenType::FOLLOWS_A,
};

set<TokenType> validStatementRef = {
    TokenType::SYNONYM,
    TokenType::WILDCARD,
    TokenType::STATEMENT_NUM,
    TokenType::NUMBER
};

set<TokenType> validEntityRef = {
    TokenType::SYNONYM,
    TokenType::WILDCARD,
    TokenType::STRING,
};

set<TokenType> validExpressionSpec = {
    TokenType::WILDCARD_STRING,
	TokenType::WILDCARD,
    TokenType::STRING,
};
