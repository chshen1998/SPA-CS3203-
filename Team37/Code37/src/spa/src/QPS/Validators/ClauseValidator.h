#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "../QPS.h"

#ifndef TEAM37_CLAUSEVALIDATOR_H
#define TEAM37_CLAUSEVALIDATOR_H

class ClauseValidator
{
public:
	void validateSuchThat(PqlToken such, PqlToken that);
	void validateBrackets(PqlToken open, PqlToken comma, PqlToken close);
	virtual void validate(PqlToken left, PqlToken right) = 0;

protected:
	ClauseValidator(unordered_map<string, TokenType> declarationsMap);

	void validateParameters(PqlToken left, PqlToken right, set<TokenType> validTypes, string clauseType);
	bool isDeclared(PqlToken synonym);

	unordered_map<string, TokenType> declarations;
};

#endif //TEAM37_CLAUSEVALIDATOR_H
