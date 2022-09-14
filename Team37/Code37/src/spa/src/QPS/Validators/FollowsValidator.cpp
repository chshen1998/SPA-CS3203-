using namespace std;

#include <unordered_map>

#include "../QPS.h"
#include "FollowsValidator.h"
#include "ClauseValidator.h"
#include "ValidatorUtils.h"

FollowsValidator::FollowsValidator(unordered_map<string, TokenType> declarations) : ClauseValidator(declarations) {}

void FollowsValidator::validate(PqlToken left, PqlToken right)
{
	validateParameters(left, right, validStatementRef, validStatementRef, "follows");
}
