using namespace std;

#include <unordered_map>

#include "../Structures/PqlError.h"
#include "../Structures/PqlToken.h"
#include "../Structures/PqlQuery.h"
#include "../Types/ErrorType.h"
#include "../Types/TokenType.h"
#include "FollowsValidator.h"
#include "ClauseValidator.h"
#include "ValidatorUtils.h"

FollowsValidator::FollowsValidator(unordered_map<string, TokenType> declarations) : ClauseValidator(declarations) {}

void FollowsValidator::validate(PqlToken left, PqlToken right)
{
	validateParameters(left, right, validStatementRef, validStatementRef, "follows");
}
