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
	void validateOpen(PqlToken token);
	void validateClose(PqlToken token);
	void validateComma(PqlToken token);
	virtual void validate(PqlToken left, PqlToken right) = 0;

protected:
	ClauseValidator(unordered_map<string, TokenType> *declarationsMap, TokenType token);

	void validateEntityRef(PqlToken token, TokenType entityType);
	void validateStatementRef(PqlToken token);
	bool isDeclared(PqlToken synonym);

	unordered_map<string, TokenType> *declarations;
	TokenType validatorType;
};

#endif //TEAM37_CLAUSEVALIDATOR_H
