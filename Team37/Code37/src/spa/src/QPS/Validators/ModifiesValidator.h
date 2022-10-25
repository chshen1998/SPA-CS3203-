#pragma once

#ifndef TEAM37_MODIFIESVALIDATOR_H
#define TEAM37_MODIFIESVALIDATOR_H

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

extern set<TokenType> validModifiesStmtParamTypes;

class ModifiesValidator : public ClauseValidator {
public:
	ModifiesValidator(unordered_map<string, TokenType> declarations, TokenType token);
	void validate(PqlToken left, PqlToken right);
};


#endif //TEAM37_MODIFIESVALIDATOR_H