using namespace std;

#include <string>

#include "ClauseValidator.h"
#include "BaseValidator.h"
#include "ValidatorUtils.h"
#include "../Structures/PqlError.h"
#include "../Structures/PqlToken.h"
#include "../Structures/PqlQuery.h"
#include "../Types/ErrorType.h"
#include "../Types/TokenType.h"

ClauseValidator::ClauseValidator(unordered_map<string, TokenType>* declarationsMap, TokenType token) : BaseValidator{}
{
	declarations = declarationsMap;
	validatorType = token;
}

void ClauseValidator::validateOpen(PqlToken token) {
	if (token.type != TokenType::OPEN_BRACKET) {
		throw SyntaxError("Invalid " + relationshipToStringMap[validatorType] + " clause missing open bracket");
	}
}

void ClauseValidator::validateClose(PqlToken token) {
	if (token.type != TokenType::CLOSED_BRACKET) {
		throw SyntaxError("Invalid " + relationshipToStringMap[validatorType] + " clause missing closed bracket");
	}
}

void ClauseValidator::validateComma(PqlToken token) {
	if (token.type != TokenType::COMMA) {
		throw SyntaxError("Invalid " + relationshipToStringMap[validatorType] + " clause missing comma between parameters");
	}
}

void ClauseValidator::validateEntityRef(PqlToken token, TokenType entityType)
{
	if (validEntityRef.find(token.type) == validEntityRef.end())
	{
		throw SyntaxError("Invalid parameters for " + relationshipToStringMap[validatorType] + " clause");
	}
	else if (token.type == TokenType::SYNONYM && !isDeclared(token))
	{
		throw SemanticError(token.value + " is undeclared parameter for " + relationshipToStringMap[validatorType] + " clause");
	} 
	else if (token.type == TokenType::SYNONYM && declarations->at(token.value) != entityType)
	{
		throw SemanticError(token.value + " is invalid parameter type for " + relationshipToStringMap[validatorType] + " clause");
	}
}

void ClauseValidator::validateStatementRef(PqlToken token)
{
	if (validStatementRef.find(token.type) == validStatementRef.end())
	{
		throw SyntaxError("Invalid parameters for " + relationshipToStringMap[validatorType] + " clause");
	}
	else if (token.type == TokenType::SYNONYM && !isDeclared(token))
	{
		throw SemanticError(token.value + " is undeclared parameter for " + relationshipToStringMap[validatorType] + " clause");
	}
	else if (token.type == TokenType::SYNONYM && statementTypes.find(declarations->at(token.value)) == statementTypes.end())
	{
		throw SemanticError(token.value + " is invalid parameter type for " + relationshipToStringMap[validatorType] + " clause");
	}
}