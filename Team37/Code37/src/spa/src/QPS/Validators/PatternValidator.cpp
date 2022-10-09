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
		throw SemanticError("Invalid pattern: " + pattern.value);
	}
}

void PatternValidator::validate(PqlToken left, PqlToken right)
{
	validateEntityRef(left, "pattern");
	validateExpressionSpec(right);
}

void PatternValidator::validateWhile(PqlToken left, PqlToken right) {
	validateEntityRef(left, "pattern");
	validateWildcard(right);
}

void PatternValidator::validateIf(PqlToken left, PqlToken mid, PqlToken right) {
	validateEntityRef(left, "pattern");
	validateWildcard(mid);
	validateWildcard(right);
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

void PatternValidator::validateWildcard(PqlToken token) {
	if (token.type != TokenType::WILDCARD) {
		throw SemanticError("Invalid parameters for pattern clause");
	}
}