using namespace std;

#include <unordered_map>

#include "../QPS.h"
#include "ParentValidator.h"
#include "ClauseValidator.h"
#include "ValidatorUtils.h"

ParentValidator::ParentValidator(unordered_map<string, TokenType> declarations) : ClauseValidator(declarations) {}

void ParentValidator::validate(PqlToken left, PqlToken right)
{
	validateParameters(left, right, validStatementRef, validStatementRef, "parent");
}
