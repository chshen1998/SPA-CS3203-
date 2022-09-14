#pragma once

#include <memory>
#include <string>
#include <set>
#include <vector>
#include <unordered_map>

#include "../QPS.h"
#include "ClauseValidator.h"

#ifndef TEAM37_SELECTVALIDATOR_H
#define TEAM37_SELECTVALIDATOR_H


class SelectValidator : public ClauseValidator {
public:
	SelectValidator(unordered_map<string, TokenType> declarations);
	void validate(PqlToken declarationType, PqlToken synonym);
};


#endif //TEAM37_SELECTVALIDATOR_H