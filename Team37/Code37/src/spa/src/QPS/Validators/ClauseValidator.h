#pragma once

#ifndef TEAM37_CLAUSEVALIDATOR_H
#define TEAM37_CLAUSEVALIDATOR_H

#include <string>
#include <vector>
#include <unordered_map>
#include <set>
#include <memory>

#include "../Structures/PqlError.h"
#include "../Structures/PqlToken.h"
#include "../Structures/PqlQuery.h"
#include "../Types/ErrorType.h"
#include "../Types/TokenType.h"
#include "ValidatorUtils.h"


class ClauseValidator
{
public:
	void validateBrackets(PqlToken open, PqlToken comma, PqlToken close);
	virtual void validate(PqlToken left, PqlToken right) = 0;

protected:
	ClauseValidator(unordered_map<string, TokenType> declarationsMap, TokenType token);

	void validateParameters(PqlToken left, PqlToken right, set<TokenType> validLeftTypes, set<TokenType> validRightTypes, string clauseType);
	bool isDeclared(PqlToken synonym);

	unordered_map<string, TokenType> declarations;
	TokenType validatorType;
};

#endif //TEAM37_CLAUSEVALIDATOR_H
