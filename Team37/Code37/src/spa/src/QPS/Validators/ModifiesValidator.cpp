using namespace std;

#include <unordered_map>

#include "../Structures/PqlError.h"
#include "../Structures/PqlToken.h"
#include "../Structures/PqlQuery.h"
#include "../Types/ErrorType.h"
#include "../Types/TokenType.h"
#include "ModifiesValidator.h"
#include "ClauseValidator.h"
#include "ValidatorUtils.h"

ModifiesValidator::ModifiesValidator(unordered_map<string, TokenType> declarations, TokenType token) : ClauseValidator(declarations, token) {}

void ModifiesValidator::validate(PqlToken left, PqlToken right)
{
	validateParameters(left, right, validStatementRef, validEntityRef, "modifies");
	if (left.type == TokenType::WILDCARD) {
		throw SemanticError("Modifies clause arg1 cannot be wildcard");
	}
}
