#pragma once

#include <memory>
#include <string>
#include <set>
#include <vector>
#include <unordered_map>

#include "../QPS.h"
#include "ClauseValidator.h"

#ifndef TEAM37_MODIFIESVALIDATOR_H
#define TEAM37_MODIFIESVALIDATOR_H


class ModifiesValidator : public ClauseValidator {
public:
	ModifiesValidator(unordered_map<string, TokenType> declarations);
	void validate(PqlToken left, PqlToken right);
};


#endif //TEAM37_MODIFIESVALIDATOR_H