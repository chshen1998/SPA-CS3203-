#pragma once

#ifndef TEAM37_SELECTVALIDATOR_H
#define TEAM37_SELECTVALIDATOR_H

#include <memory>
#include <string>
#include <set>
#include <vector>
#include <unordered_map>

#include "../Structures/PqlError.h"
#include "../Structures/PqlToken.h"
#include "../Structures/PqlQuery.h"
#include "../Types/ErrorType.h"
#include "../Types/TokenType.h"
#include "ClauseValidator.h"



class SelectValidator {
public:
	SelectValidator(unordered_map<string, TokenType> declarations);
	void validateSelect(PqlToken select);
	void validateSynonym(PqlToken token);
	void validateAttrName(PqlToken dot, PqlToken attrName);
	void validateMultiple(vector<PqlToken> tokens);

private:
	void validateSingle(int start, int end);
	bool isDeclared(PqlToken token);
	PqlToken getNextToken();

	unordered_map<string, TokenType> declarations;
	vector<PqlToken> tokens;
	int next;
	int size;
};


#endif //TEAM37_SELECTVALIDATOR_H