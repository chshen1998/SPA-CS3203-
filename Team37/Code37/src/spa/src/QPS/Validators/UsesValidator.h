#pragma once

#ifndef TEAM37_USESVALIDATOR_H
#define TEAM37_USESVALIDATOR_H

#include <memory>
#include <string>
#include <set>
#include <vector>
#include <unordered_map>

#include "../QPS.h"
#include "ClauseValidator.h"

class UsesValidator : public ClauseValidator {
public:
	UsesValidator(unordered_map<string, TokenType> declarations);
	void validate(PqlToken left, PqlToken right);
};


#endif //TEAM37_USESVALIDATOR_H