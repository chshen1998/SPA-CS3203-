using namespace std;

#include <unordered_map>

#include "../QPS.h"
#include "PatternValidator.h"
#include "ClauseValidator.h"
#include "ValidatorUtils.h"

PatternValidator::PatternValidator(unordered_map<string, TokenType> declarations) : ClauseValidator(declarations) {}

void PatternValidator::validateAssign(PqlToken assign)
{
	if (assign.type != TokenType::SYNONYM || !isDeclared(assign) || declarations[assign.value] != TokenType::ASSIGN)
	{
		throw SemanticError("Pattern clause must be followed by assign synonym");
	}
}

void PatternValidator::validate(PqlToken left, PqlToken right)
{
	validateParameters(left, right, validPatternParameterTypes, "pattern");
}
