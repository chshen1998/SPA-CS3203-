using namespace std;

#include <unordered_map>

#include "../Structures/PqlError.h"
#include "../Structures/PqlQuery.h"
#include "../Structures/PqlToken.h"
#include "../Types/ErrorType.h"
#include "../Types/TokenType.h"
#include "ClauseValidator.h"
#include "FollowsValidator.h"
#include "ValidatorUtils.h"

FollowsValidator::FollowsValidator(unordered_map<string, TokenType>* declarations, TokenType token)
    : ClauseValidator(declarations, token)
{
}

void FollowsValidator::validate(PqlToken* left, PqlToken* right)
{
    validateStatementRef(left);
    validateStatementRef(right);
}
