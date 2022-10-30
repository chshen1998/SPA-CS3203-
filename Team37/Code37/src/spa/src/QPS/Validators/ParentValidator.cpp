using namespace std;

#include <unordered_map>

#include "../Structures/PqlError.h"
#include "../Structures/PqlToken.h"
#include "../Structures/PqlQuery.h"
#include "../Types/ErrorType.h"
#include "../Types/TokenType.h"
#include "ParentValidator.h"
#include "ClauseValidator.h"
#include "ValidatorUtils.h"

ParentValidator::ParentValidator(unordered_map<string, TokenType> *declarations, TokenType token) : ClauseValidator(declarations, token) {}

void ParentValidator::validate(PqlToken left, PqlToken right)
{
	validateStatementRef(left);
	validateStatementRef(right);
}
