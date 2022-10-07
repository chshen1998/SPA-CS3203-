#pragma once

#ifndef TEAM37_PATTERNVALIDATOR_H
#define TEAM37_PATTERNVALIDATOR_H

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

class PatternValidator : public ClauseValidator {
public:
	PatternValidator(unordered_map<string, TokenType> declarations, TokenType token);
	void validatePattern(PqlToken assign);
	void validate(PqlToken left, PqlToken right);
	void validateWhile(PqlToken left, PqlToken right);
	void validateIf(PqlToken left, PqlToken mid, PqlToken right);

private:
	void validateExpressionSpec(PqlToken token);
	void validateWildcard(PqlToken token);
};


#endif //TEAM37_PATTERNVALIDATOR_H
