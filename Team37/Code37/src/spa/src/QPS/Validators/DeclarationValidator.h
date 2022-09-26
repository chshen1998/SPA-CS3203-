#pragma once

#ifndef TEAM37_DECLARATIONVALIDATOR_H
#define TEAM37_DECLARATIONVALIDATOR_H

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


class DeclarationValidator {
public:
	DeclarationValidator(vector<PqlToken> tokens);
	unordered_map<string, TokenType> validate();

private:
	void isValidDesignEntity(PqlToken token);
	void isSynonym(PqlToken token);
	void isSemicolonOrComma(PqlToken token);
	PqlToken getNextToken();

	int next;
	int size;
	vector<PqlToken> tokens;
};


#endif //TEAM37_DECLARATIONVALIDATOR_H
