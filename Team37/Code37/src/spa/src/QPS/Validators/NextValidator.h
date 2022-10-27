#pragma once

#include "../Structures/PqlError.h"
#include "../Structures/PqlToken.h"
#include "../Structures/PqlQuery.h"
#include "../Types/ErrorType.h"
#include "../Types/TokenType.h"
#include "ClauseValidator.h"

#include <memory>
#include <string>
#include <set>
#include <vector>
#include <unordered_map>

#ifndef TEAM37_NEXTVALIDATOR_H
#define TEAM37_NEXTVALIDATOR_H


class NextValidator : public ClauseValidator {
public:
	NextValidator(unordered_map<string, TokenType> *declarations, TokenType token);
	void validate(PqlToken left, PqlToken right);
};


#endif //TEAM37_NEXTVALIDATOR_H