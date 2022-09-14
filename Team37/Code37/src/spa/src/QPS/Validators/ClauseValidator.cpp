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

void ClauseValidator::validateSuchThat(PqlToken such, PqlToken that)
{
	if (such.type == TokenType::SUCH && that.type == TokenType::THAT)
	{
		throw SyntaxError("The keywords 'such that' must be used prior to a relationship reference");
	}
}

void ClauseValidator::validateBrackets(PqlToken open, PqlToken comma, PqlToken close)
{
	if (open.type == TokenType::OPEN_BRACKET && comma.type == TokenType::COMMA && close.type == TokenType::CLOSED_BRACKET)
	{
		throw SyntaxError("The parameters passed to a clause must be enclosed within brackets and separated by a comma");
	}
}

void ClauseValidator::validateParameters(PqlToken left, PqlToken right, set<TokenType> validLeftTypes, set<TokenType> validRightTypes, string clauseType)
{
	if (!(validLeftTypes.find(left.type) != validLeftTypes.end()) || !(validRightTypes.find(right.type) != validRightTypes.end()))
	{
		throw SemanticError("Invalid parameters for " + clauseType + " clause");
	}
	else if (!isDeclared(left) || !isDeclared(right))
	{
		throw SemanticError("Undeclared parameters for " + clauseType + " clause");
	}
}

bool ClauseValidator::isDeclared(PqlToken synonym)
{
	auto findit = declarations.find(synonym.value);
	return (findit != declarations.end());
}

