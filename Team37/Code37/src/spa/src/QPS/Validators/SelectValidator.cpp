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

SelectValidator::SelectValidator(unordered_map<string, TokenType>* declarationMap) : BaseValidator {}
{
	declarations = declarationMap;
}

void SelectValidator::validateSelect(PqlToken select) 
{
	if (select.type != TokenType::SELECT)
	{
		throw SyntaxError("Select clause must come after declarations");
	}
}

void SelectValidator::validateMultiple(vector<PqlToken> *tokenVector) 
{
	tokens = tokenVector;
	next = 0;
	size = tokenVector->size();

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
		if (tokens->at(start).type != TokenType::BOOLEAN) {
			validateSynonym(tokens->at(start));
		}
	}
	else if (end - start == 3) {
		validateSynonym(tokens->at(start));
		validateAttrName(tokens->at(start+1), tokens->at(start+2));
		validateAttrNameMatch(tokens->at(start), tokens->at(start + 2));
	}
	else {
		throw SyntaxError("Invalid Select clause parameter");
	}
}

void SelectValidator::validateSynonym(PqlToken token) {
	if (token.type != TokenType::SYNONYM) {
		throw SyntaxError("Invalid Select clause parameter");
	}
	else if (!isDeclared(token)) {
		throw SemanticError("Undeclared synonym in Select clause");
	}
}

void SelectValidator::validateAttrName(PqlToken dot, PqlToken attrName) {
	if (dot.type != TokenType::DOT) {
		throw SyntaxError("Invalid Select clause parameter - Synonym and AttrName must be seperated by '.'");
	} 
	else if (validAttrName.find(attrName.type) == validAttrName.end()) {
		throw SyntaxError("Invalid Select clause parameter - Invalid AttrName");
	}
}

void SelectValidator::validateAttrNameMatch(PqlToken synonym, PqlToken attrName) {
	TokenType t = declarations->at(synonym.value);
	if (validSynonymToAttrMap[t].find(attrName.type) == validSynonymToAttrMap[t].end()) {
		throw SemanticError("Invalid attrName for attrRef synonym");
	}
}