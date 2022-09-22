#pragma once

#ifndef TEAM37_PATTERNVALIDATOR_H
#define TEAM37_PATTERNVALIDATOR_H

#include <memory>
#include <string>
#include <set>
#include <vector>
#include <unordered_map>

#include "../QPS.h"
#include "ClauseValidator.h"

class PatternValidator : public ClauseValidator {
public:
	PatternValidator(unordered_map<string, TokenType> declarations);
	void validateAssign(PqlToken assign);
	void validate(PqlToken left, PqlToken right);
};


#endif //TEAM37_PATTERNVALIDATOR_H
