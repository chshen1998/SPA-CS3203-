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

void PatternValidator::validateAssign(PqlToken assign)
{
	if (assign.type != TokenType::SYNONYM || !isDeclared(assign) || declarations[assign.value] != TokenType::ASSIGN)
	{
		throw SemanticError("Pattern clause must be followed by a declared assign synonym");
	}
}

void PatternValidator::validate(PqlToken left, PqlToken right)
{
	validateParameters(left, right, validEntityRef, validExpressionSpec, "pattern");
}
