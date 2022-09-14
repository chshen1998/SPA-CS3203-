#pragma once

#include <memory>
#include <string>
#include <set>
#include <vector>
#include <unordered_map>

#include "../QPS.h"
#include "ClauseValidator.h"

#ifndef TEAM37_PATTERNVALIDATOR_H
#define TEAM37_PATTERNVALIDATOR_H


class PatternValidator : public ClauseValidator {
public:
	PatternValidator(unordered_map<string, TokenType> declarations);
	PqlError validateAssign(PqlToken assign);
	PqlError validateParameters(PqlToken left, PqlToken right);

private:
	bool isValidParameter(PqlToken token);
};


#endif //TEAM37_PATTERNVALIDATOR_H
