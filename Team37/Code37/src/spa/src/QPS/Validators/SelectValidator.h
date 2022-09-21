#pragma once

#ifndef TEAM37_SELECTVALIDATOR_H
#define TEAM37_SELECTVALIDATOR_H

#include <memory>
#include <string>
#include <set>
#include <vector>
#include <unordered_map>

#include "../QPS.h"
#include "ClauseValidator.h"



class SelectValidator {
public:
	SelectValidator(unordered_map<string, TokenType> declarations);
	void validate(PqlToken declarationType, PqlToken synonym);

private:
	unordered_map<string, TokenType> declarations;
};


#endif //TEAM37_SELECTVALIDATOR_H