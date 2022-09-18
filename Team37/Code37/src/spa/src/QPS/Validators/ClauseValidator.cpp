using namespace std;

#include <string>
#include <vector>
#include <unordered_map>

#include "../QPS.h"
#include "ClauseValidator.h"
#include "ValidatorUtils.h"

ClauseValidator::ClauseValidator(unordered_map<string, TokenType> declarationsMap)
{
	declarations = declarationsMap;
}

void ClauseValidator::validateBrackets(PqlToken open, PqlToken comma, PqlToken close)
{
	if (!(open.type == TokenType::OPEN_BRACKET) || !(comma.type == TokenType::COMMA) || !(close.type == TokenType::CLOSED_BRACKET))
	{
		throw SyntaxError("The parameters passed to a clause must be enclosed within brackets and separated by a comma");
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

bool ClauseValidator::isDeclared(PqlToken synonym)
{
	auto findit = declarations.find(synonym.value);
	return (findit != declarations.end());
}

