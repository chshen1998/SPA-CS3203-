using namespace std;

#include <string>

#include "ClauseValidator.h"
#include "ValidatorUtils.h"
#include "../Structures/PqlError.h"
#include "../Structures/PqlToken.h"
#include "../Structures/PqlQuery.h"
#include "../Types/ErrorType.h"
#include "../Types/TokenType.h"

ClauseValidator::ClauseValidator(unordered_map<string, TokenType> declarationsMap, TokenType token)
{
	declarations = declarationsMap;
	validatorType = token;
}

void ClauseValidator::validateBrackets(PqlToken open, PqlToken comma, PqlToken close)
{
	if (!(open.type == TokenType::OPEN_BRACKET) || !(comma.type == TokenType::COMMA) || !(close.type == TokenType::CLOSED_BRACKET))
	{
		throw SyntaxError( relationshipToStringMap[validatorType] + " clause parameters must be enclosed within brackets and separated by a comma");
	}
}

void ClauseValidator::validateParameters(PqlToken left, PqlToken right, set<TokenType> validLeftTypes, set<TokenType> validRightTypes, string clauseType)
{
	if (!(validLeftTypes.find(left.type) != validLeftTypes.end()) || !(validRightTypes.find(right.type) != validRightTypes.end()))
	{
		throw SemanticError("Invalid parameters for " + clauseType + " clause");
	} else if (left.type == TokenType::SYNONYM && !isDeclared(left))
	{
		throw SemanticError(left.value + " is undeclared parameter for " + clauseType + " clause");
	}
	else if (right.type == TokenType::SYNONYM && !isDeclared(right))
	{
		throw SemanticError(right.value + " is undeclared parameter for " + clauseType + " clause");
	}
}

void ClauseValidator::validateEntityRef(PqlToken token, string clauseType)
{
	if (validEntityRef.find(token.type) == validEntityRef.end())
	{
		throw SemanticError("Invalid parameters for " + clauseType + " clause");
	}
	else if (token.type == TokenType::SYNONYM && !isDeclared(token))
	{
		throw SemanticError(token.value + " is undeclared parameter for " + clauseType + " clause");
	} 
}

void ClauseValidator::validateStatementRef(PqlToken token, string clauseType)
{
	if (validStatementRef.find(token.type) == validStatementRef.end())
	{
		throw SemanticError("Invalid parameters for " + clauseType + " clause");
	}
	else if (token.type == TokenType::SYNONYM && !isDeclared(token))
	{
		throw SemanticError(token.value + " is undeclared parameter for " + clauseType + " clause");
	}
	else if (token.type == TokenType::WILDCARD_STRING)
	{
		// Implement wildcard string validation
	}
}

bool ClauseValidator::isDeclared(PqlToken synonym)
{
	auto findit = declarations.find(synonym.value);
	return (findit != declarations.end());
}

