using namespace std;

#include <set>
#include <unordered_map>

#include "../QPS.h"
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
    TokenType::WILDCARD
};


/*
 * Set of valid token types for pattern clause parameters
 */
set<TokenType> validPatternParameterTypes = {
        TokenType::SYNONYM,
        TokenType::CONSTANT,
        TokenType::STRING,
        TokenType::NUMBER
};

/*
 * Set of valid token types for follows clause parameters
 */
set<TokenType> validFollowsParameterTypes = {
    TokenType::SYNONYM,
    TokenType::STATEMENT_NUM
};


/*
 * Set of valid token types for uses clause parameters
 */
set<TokenType> validUsesParameterTypes = {
    TokenType::SYNONYM,
    TokenType::STATEMENT_NUM
};


/*
 * Set of valid token types for Modifies clause parameters
 */
set<TokenType> validModifiesParameterTypes = {
    TokenType::SYNONYM,
    TokenType::STATEMENT_NUM
};


/*
 * Set of valid token types for Parent clause parameters
 */
set<TokenType> validParentParameterTypes = {
    TokenType::SYNONYM,
    TokenType::STATEMENT_NUM
};