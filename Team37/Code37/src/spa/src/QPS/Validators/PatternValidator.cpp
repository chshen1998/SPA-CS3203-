using namespace std;

#include <unordered_map>

#include "../Structures/PqlError.h"
#include "../Structures/PqlToken.h"
#include "../Structures/PqlQuery.h"
#include "../Types/ErrorType.h"
#include "../Types/TokenType.h"
#include "PatternValidator.h"
#include "ClauseValidator.h"
#include "ValidatorUtils.h"

PatternValidator::PatternValidator(unordered_map<string, TokenType> declarations, TokenType token) : ClauseValidator(declarations, token) {}

void PatternValidator::validatePattern(PqlToken pattern)
{
	if (pattern.type != TokenType::SYNONYM || !isDeclared(pattern) || validPatternType.find(declarations[pattern.value]) == validPatternType.end())
	{
		throw SemanticError(pattern.value + " is not a valid pattern");
	}
}

void PatternValidator::validate(PqlToken left, PqlToken right)
{
	validateEntityRef(left, "pattern");
}

void PatternValidator::validateExpressionSpec(PqlToken token)
{
	if (validExpressionSpec.find(token.type) == validExpressionSpec.end())
	{
		throw SemanticError("Invalid parameters for pattern clause");
	}
	else if (token.type == TokenType::WILDCARD_STRING)
	{
		// Implement wildcard string validation
	}
}