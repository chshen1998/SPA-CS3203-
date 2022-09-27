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
    TokenType::STATEMENT,
    TokenType::READ,
    TokenType::PRINT,
    TokenType::CALL,
    TokenType::WHILE,
    TokenType::IF,
    TokenType::ASSIGN,
	TokenType::VARIABLE,
    TokenType::CONSTANT,
    TokenType::PROCEDURE
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
    TokenType::CALLS,
    TokenType::CALLS_A,
    TokenType::NEXT,
    TokenType::NEXT_A,
    TokenType::AFFECTS,
    TokenType::AFFECTS_A
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

set<TokenType> validAttrName = {
    TokenType::PROCNAME,
    TokenType::VARNAME,
    TokenType::VALUE,
    TokenType::STATEMENT_NUM
};

set<TokenType> validPatternType = {
    TokenType::ASSIGN,
    TokenType::WHILE,
    TokenType::IF
};

unordered_map<TokenType, string> relationshipToStringMap = {
    {TokenType::USES, "uses"},
    {TokenType::USES_P, "uses"},
    {TokenType::MODIFIES, "modifies"},
    {TokenType::MODIFIES_P, "modifies"},
    {TokenType::PARENT, "parent"},
    {TokenType::PARENT_A, "parent*"},
    {TokenType::FOLLOWS, "follows"},
    {TokenType::FOLLOWS_A, "follows*"},
    {TokenType::CALLS, "calls"},
    {TokenType::CALLS_A, "calls*"},
    {TokenType::NEXT, "next"},
    {TokenType::NEXT_A, "next*"},
    {TokenType::AFFECTS, "affects"},
    {TokenType::AFFECTS_A, "affects*"}
};
