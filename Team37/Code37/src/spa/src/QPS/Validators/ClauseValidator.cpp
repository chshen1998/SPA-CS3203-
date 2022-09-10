using namespace std;

#include <string>
#include <vector>
#include <unordered_map>

#include "../QPS.h"
#include "ClauseValidator.h"

ClauseValidator::ClauseValidator(unordered_map<string, TokenType> declarations)
{
	pe.type = ErrorType::NONE;
	declarations = declarations;
}

PqlError ClauseValidator::validateSuchThat(PqlToken such, PqlToken that)
{
	if (such.type == TokenType::SUCH && that.type == TokenType::THAT)
	{
		updatePqlError(ErrorType::SYNTAX_ERROR, "Keywords such that must be used prior to a SuchThatClause");
	}
	return pe;
}

PqlError ClauseValidator::validateBrackets(PqlToken open, PqlToken comma, PqlToken close)
{
	if (open.type == TokenType::OPEN_BRACKET && comma.type == TokenType::COMMA && close.type == TokenType::CLOSED_BRACKET)
	{
		updatePqlError(ErrorType::SYNTAX_ERROR, "The parameters passed to a clause must be enclosed within brackets and seperated by a comma");
	}
	return pe;
}

bool ClauseValidator::isDeclared(PqlToken synonym)
{
	if (declarations.find(synonym.value) != declarations.end())
	{
		return true;
	}
	return false;
}

bool ClauseValidator::isTokenType(PqlToken synonym, TokenType type)
{
	if (declarations[synonym.value] == type)
	{
		return true;
	}
	return false;
}

void ClauseValidator::updatePqlError(ErrorType type, string msg)
{
	pe.type = type;
	pe.message = msg;
}
