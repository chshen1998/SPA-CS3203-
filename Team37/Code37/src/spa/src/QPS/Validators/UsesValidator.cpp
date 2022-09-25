using namespace std;

#include <unordered_map>

#include "../Structures/PqlError.h"
#include "../Structures/PqlToken.h"
#include "../Structures/PqlQuery.h"
#include "../Types/ErrorType.h"
#include "../Types/TokenType.h"
#include "UsesValidator.h"
#include "ClauseValidator.h"
#include "ValidatorUtils.h"

UsesValidator::UsesValidator(unordered_map<string, TokenType> declarations) : ClauseValidator(declarations) {}

void UsesValidator::validate(PqlToken left, PqlToken right)
{
	validateParameters(left, right, validStatementRef, validEntityRef, "uses");
	if (left.type == TokenType::WILDCARD) {
		throw SemanticError("Uses clause arg1 cannot be wildcard");
	}
}
