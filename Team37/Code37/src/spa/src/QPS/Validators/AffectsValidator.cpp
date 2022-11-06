using namespace std;

#include <unordered_map>

#include "../Structures/PqlError.h"
#include "../Structures/PqlToken.h"
#include "../Structures/PqlQuery.h"
#include "../Types/ErrorType.h"
#include "../Types/TokenType.h"
#include "AffectsValidator.h"
#include "ClauseValidator.h"
#include "ValidatorUtils.h"

AffectsValidator::AffectsValidator(unordered_map<string, TokenType> *declarations, TokenType token) : ClauseValidator(declarations, token) {}

void AffectsValidator::validate(PqlToken *left, PqlToken *right)
{
	validateStatementRef(left);
	validateStatementRef(right);
}
