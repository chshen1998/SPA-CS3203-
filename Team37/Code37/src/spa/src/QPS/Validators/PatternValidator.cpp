using namespace std;

#include <unordered_map>

#include "../QPS.h"
#include "PatternValidator.h"
#include "ClauseValidator.h"

PatternValidator::PatternValidator(unordered_map<string, TokenType> declarations) : ClauseValidator(declarations) {}

PqlError PatternValidator::validateAssign(PqlToken assign)
{
	if (assign.type != TokenType::SYNONYM || !isDeclared(assign) || !isTokenType(assign, TokenType::ASSIGN))
	{
		updatePqlError(ErrorType::SEMANTIC_ERROR, "Pattern clause must be followed by valid synonym");
	}
	return pe;
}

PqlError PatternValidator::validateParameters(PqlToken left, PqlToken right)
{
	if (!isValidParameter(left) || !isValidParameter(right))
	{
		updatePqlError(ErrorType::SEMANTIC_ERROR, "Invalid parameters for pattern clause");
	} else if (!isDeclared(left) || !isDeclared(right))
	{
		updatePqlError(ErrorType::SEMANTIC_ERROR, "Undeclared parameters for pattern clause");
	}
	return pe;
}

bool PatternValidator::isValidParameter(PqlToken token)
{
	if (validPatternParameters.find(token.type) != validPatternParameters.end())
	{
		return true;
	}
	return false;
}
