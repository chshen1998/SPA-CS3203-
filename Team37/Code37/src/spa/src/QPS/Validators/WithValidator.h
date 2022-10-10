#pragma once

#ifndef TEAM37_WITHVALIDATOR_H
#define TEAM37_WITHVALIDATOR_H

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

class WithValidator {
public:
	WithValidator(unordered_map<string, TokenType> declarationsMap, vector<PqlToken> withTokens);
	void validate();

private:
	TokenType validateRef(vector<PqlToken> refTokens);
	PqlToken getNextToken();

	unordered_map<string, TokenType> declarations;
	vector<PqlToken> tokens;
	int next;
	int size;
};


#endif //TEAM37_WITHVALIDATOR_H