using namespace std;

#include <unordered_map>

#include "../QPS.h"
#include "ModifiesValidator.h"
#include "ClauseValidator.h"
#include "ValidatorUtils.h"

ModifiesValidator::ModifiesValidator(unordered_map<string, TokenType> declarations) : ClauseValidator(declarations) {}

void ModifiesValidator::validate(PqlToken left, PqlToken right)
{
	validateParameters(left, right, validStatementRef, validEntityRef, "modifies");
	if (left.type == TokenType::WILDCARD) {
		throw SemanticError("Modifies clause arg1 cannot be wildcard");
	}
}
