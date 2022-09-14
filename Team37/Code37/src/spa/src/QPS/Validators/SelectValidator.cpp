using namespace std;

#include <unordered_map>

#include "../QPS.h"
#include "SelectValidator.h"
#include "ClauseValidator.h"
#include "ValidatorUtils.h"

SelectValidator::SelectValidator(unordered_map<string, TokenType> declarations) : ClauseValidator(declarations) {}

void SelectValidator::validate(PqlToken declarationType, PqlToken synonym)
{
	if (declarationType.type != TokenType::SELECT)
	{
		throw SemanticError("Select clause must come after declarations");
	} else if ((synonym.type != TokenType::SYNONYM))
	{
		throw SemanticError("Select clause must be followed by a synonym");
	} else if (!isDeclared(synonym))
	{
		throw SemanticError("Undeclared parameter passed to select clause");
	}
}