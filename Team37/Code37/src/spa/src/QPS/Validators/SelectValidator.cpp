using namespace std;

#include <unordered_map>

#include "../QPS.h"
#include "SelectValidator.h"
#include "ClauseValidator.h"

SelectValidator::SelectValidator(unordered_map<string, TokenType> declarations) : ClauseValidator(declarations) {}

PqlError SelectValidator::validate(PqlToken declarationType, PqlToken synonym)
{
	if (declarationType.type != TokenType::SELECT)
	{
		updatePqlError(ErrorType::SEMANTIC_ERROR, "Select clause must come after declarations");
	} else if ((synonym.type != TokenType::SYNONYM) || (!isDeclared(synonym)))
	{
		updatePqlError(ErrorType::SEMANTIC_ERROR, "Select clause must be followed by a valid synonym");
	}
	return pe;
}