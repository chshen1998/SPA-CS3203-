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

set<TokenType> statementTypes = {
    TokenType::STATEMENT,
    TokenType::READ,
    TokenType::PRINT,
    TokenType::ASSIGN,
    TokenType::CALL,
    TokenType::WHILE,
    TokenType::IF
};

set<TokenType> entityTypes = {
    TokenType::VARIABLE
};

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
    TokenType::STMTLINE
};

set<TokenType> validAttrType = {
    TokenType::STRING,
    TokenType::NUMBER,
};

set<TokenType> validPatternType = {
    TokenType::ASSIGN,
    TokenType::WHILE,
    TokenType::IF
};

unordered_map<TokenType, string> relationshipToStringMap = {
    {TokenType::USES, "Uses"},
    {TokenType::USES_P, "Uses"},
    {TokenType::MODIFIES, "Modifies"},
    {TokenType::MODIFIES_P, "Modifies"},
    {TokenType::PARENT, "Parent"},
    {TokenType::PARENT_A, "Parent*"},
    {TokenType::FOLLOWS, "Follows"},
    {TokenType::FOLLOWS_A, "Follows*"},
    {TokenType::CALLS, "Calls"},
    {TokenType::CALLS_A, "Calls*"},
    {TokenType::NEXT, "Next"},
    {TokenType::NEXT_A, "Next*"},
    {TokenType::AFFECTS, "Affects"},
    {TokenType::AFFECTS_A, "Affects*"}
};

unordered_map<TokenType, set<TokenType>> validSynonymToAttrMap = {
    {TokenType::PROCEDURE, {TokenType::PROCNAME}},
    {TokenType::CALL, {TokenType::PROCNAME, TokenType::STMTLINE}},
    {TokenType::VARIABLE, {TokenType::VARNAME}},
    {TokenType::READ, {TokenType::VARNAME, TokenType::STMTLINE}},
    {TokenType::PRINT, {TokenType::VARNAME, TokenType::STMTLINE}},
    {TokenType::CONSTANT, {TokenType::VALUE}},
    {TokenType::STATEMENT, {TokenType::STMTLINE}},
    {TokenType::WHILE, {TokenType::STMTLINE}},
    {TokenType::IF, {TokenType::STMTLINE}},
    {TokenType::ASSIGN, {TokenType::STMTLINE}}
};
