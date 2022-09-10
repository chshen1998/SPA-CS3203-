#pragma once

#include <memory>
#include <string>
#include <set>
#include <vector>
#include <unordered_map>

#include "../QPS.h"
#include "ClauseValidator.h"
#include "Validator.h"

#ifndef TEAM37_DECLARATIONVALIDATOR_H
#define TEAM37_DECLARATIONVALIDATOR_H


class DeclarationValidator : Validator {
public:
	DeclarationValidator();
	PqlError validate(PqlToken declarationType, PqlToken synonym, PqlToken semicolon);

private:
	bool isValidDeclarationType(TokenType type);
};


#endif //TEAM37_DECLARATIONVALIDATOR_H
