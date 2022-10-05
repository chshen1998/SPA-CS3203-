using namespace std;

#include <unordered_map>

#include "../Structures/PqlError.h"
#include "../Structures/PqlToken.h"
#include "../Structures/PqlQuery.h"
#include "../Types/ErrorType.h"
#include "../Types/TokenType.h"
#include "NextValidator.h"
#include "ClauseValidator.h"
#include "ValidatorUtils.h"

NextValidator::NextValidator(unordered_map<string, TokenType> declarations, TokenType token) : ClauseValidator(declarations, token) {}

void NextValidator::validate(PqlToken left, PqlToken right)
{
	validateStatementRef(left, "next");
	validateStatementRef(right, "next");
}
