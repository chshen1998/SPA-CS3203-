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
	PqlError validateSuchThat(PqlToken such, PqlToken that);
	PqlError validateBrackets(PqlToken open, PqlToken comma, PqlToken close);
	virtual PqlError validateParameters(PqlToken left, PqlToken right) = 0;

protected:
	ClauseValidator(unordered_map<string, TokenType> declarationsMap);

	bool isDeclared(PqlToken synonym);
	bool isTokenType(PqlToken synonym, TokenType type);
	void updatePqlError(ErrorType type, string msg);

	unordered_map<string, TokenType> declarations;
	PqlError pe;
};

#endif //TEAM37_CLAUSEVALIDATOR_H
