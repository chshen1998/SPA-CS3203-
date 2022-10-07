using namespace std;

#include <unordered_map>
#include <vector>

#include "../Structures/PqlError.h"
#include "../Structures/PqlToken.h"
#include "../Structures/PqlQuery.h"
#include "../Types/ErrorType.h"
#include "../Types/TokenType.h"
#include "SelectValidator.h"
#include "ClauseValidator.h"
#include "ValidatorUtils.h"

SelectValidator::SelectValidator(unordered_map<string, TokenType> declarationMap)
{
	declarations = declarationMap;
}

void SelectValidator::validateSelect(PqlToken select) 
{
	if (select.type != TokenType::SELECT)
	{
		throw SemanticError("Select clause must come after declarations");
	}
}

void SelectValidator::validateMultiple(vector<PqlToken> tokenVector) 
{
	tokens = tokenVector;
	next = 0;
	size = tokenVector.size();

	PqlToken curr = getNextToken();
	int start = 0;
	int end = start;
	while (curr.type != TokenType::CLOSED_ARROW) {
		while (curr.type != TokenType::COMMA && curr.type != TokenType::CLOSED_ARROW) {
			end++;
			curr = getNextToken();
		}

		if (curr.type == TokenType::COMMA) {
			curr = getNextToken();
		}

		validateSingle(start, end);
		start = end + 1;
		end = start;
	}
}

void SelectValidator::validateSingle(int start, int end) {
	if (end-start == 1) {
		if (tokens[start].type != TokenType::BOOLEAN) {
			validateSynonym(tokens[start]);
		}
	}
	else if (end - start == 3) {
		validateSynonym(tokens[start]);
		validateAttrName(tokens[start+1], tokens[start+2]);
	}
	else {
		throw SemanticError("Invalid Select clause parameter");
	}
}

void SelectValidator::validateSynonym(PqlToken token) {
	if (token.type != TokenType::SYNONYM) {
		throw SemanticError("Invalid Select clause parameter");
	}
	else if (!isDeclared(token)) {
		throw SemanticError("Undeclared synonym in Select clause");
	}
}

void SelectValidator::validateAttrName(PqlToken dot, PqlToken attrName) {
	if (dot.type != TokenType::DOT) {
		throw SemanticError("Invalid Select clause parameter - Synonym and AttrName must be seperated by '.'");
	} 
	else if (validAttrName.find(attrName.type) == validAttrName.end()) {
		throw SemanticError("Invalid Select clause parameter - Invalid AttrName");
	}
}

bool SelectValidator::isDeclared(PqlToken synonym)
{
	auto findit = declarations.find(synonym.value);
	return (findit != declarations.end());
}

PqlToken SelectValidator::getNextToken() {
	if (next == size)
	{
		return PqlToken(TokenType::END, "");
	}
	PqlToken token = tokens[next];
	next = next + 1;
	return token;

}