#pragma once

#ifndef TEAM37_PARENTVALIDATOR_H
#define TEAM37_PARENTVALIDATOR_H

#include <memory>
#include <string>
#include <set>
#include <vector>
#include <unordered_map>

#include "../QPS.h"
#include "ClauseValidator.h"

class ParentValidator : public ClauseValidator {
public:
	ParentValidator(unordered_map<string, TokenType> declarations);
	void validate(PqlToken left, PqlToken right);
};


#endif //TEAM37_PARENTVALIDATOR_H