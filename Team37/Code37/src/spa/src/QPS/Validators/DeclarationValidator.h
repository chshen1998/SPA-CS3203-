#pragma once

#include <memory>
#include <string>
#include <set>
#include <vector>
#include <unordered_map>

#include "../QPS.h"
#include "ClauseValidator.h"

#ifndef TEAM37_DECLARATIONVALIDATOR_H
#define TEAM37_DECLARATIONVALIDATOR_H


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
