#pragma once

#ifndef TEAM37_BASEVALIDATOR_H
#define TEAM37_BASEVALIDATOR_H

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


class BaseValidator
{
protected:
	BaseValidator();

	PqlToken getNextToken();
	bool isDeclared(PqlToken *token);

	unordered_map<string, TokenType>* declarations;
	vector<PqlToken> *tokens;
	int next;
	int size;
};

#endif //TEAM37_BASEVALIDATOR_H