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

PatternValidator::PatternValidator(unordered_map<string, TokenType> declarations) : ClauseValidator(declarations) {}

void PatternValidator::validatePattern(PqlToken pattern)
{
	if (pattern.type != TokenType::SYNONYM || !isDeclared(pattern) || validPatternType.find(declarations[pattern.value]) == validPatternType.end())
	{
		throw SemanticError(pattern.value + " is not a valid pattern");
	}
}

void PatternValidator::validate(PqlToken left, PqlToken right)
{
	validateParameters(left, right, validEntityRef, validExpressionSpec, "pattern");
}
