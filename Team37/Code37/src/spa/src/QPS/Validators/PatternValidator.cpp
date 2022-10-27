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

PatternValidator::PatternValidator(unordered_map<string, TokenType> *declarations) : ClauseValidator(declarations, TokenType::PATTERN) {}

void PatternValidator::validatePattern(PqlToken pattern)
{
	if (pattern.type != TokenType::SYNONYM || !isDeclared(pattern) || validPatternType.find(declarations->at(pattern.value)) == validPatternType.end())
	{
		throw SemanticError("Invalid pattern: " + pattern.value);
	}
}

void PatternValidator::validate(PqlToken left, PqlToken right)
{
	validateEntityRef(left, entityTypes);
	validateExpressionSpec(right);
}

void PatternValidator::validateWhile(PqlToken left, PqlToken right) {
	validateEntityRef(left, entityTypes);
	validateWildcard(right);
}

void PatternValidator::validateIf(PqlToken left, PqlToken mid, PqlToken right) {
	validateEntityRef(left, entityTypes);
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
		int tokenLen = token.value.size();
		if (token.value[0] != '_' || token.value[tokenLen - 1] != '_') {
			throw SemanticError(token.value + " is invalid wildcard matching");
		}
	}
}

void PatternValidator::validateWildcard(PqlToken token) {
	if (token.type != TokenType::WILDCARD) {
		throw SemanticError("Invalid parameters for pattern clause");
	}
}