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
#include "BaseValidator.h"


class DeclarationValidator : public BaseValidator {
public:
	DeclarationValidator(vector<PqlToken> *tokens, unordered_map<string, TokenType> *map);
	void validate();

private:
	void isValidDesignEntity(PqlToken token);
	void isValidSynonym(PqlToken token);
	void isSemicolonOrComma(PqlToken token);
};


#endif //TEAM37_DECLARATIONVALIDATOR_H
