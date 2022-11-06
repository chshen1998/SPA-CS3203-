#pragma once

#include "../Structures/PqlError.h"
#include "../Structures/PqlQuery.h"
#include "../Structures/PqlToken.h"
#include "../Types/ErrorType.h"
#include "../Types/TokenType.h"
#include "ClauseValidator.h"

#include <memory>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

#ifndef TEAM37_AFFECTSVALIDATOR_H
#define TEAM37_AFFECTSVALIDATOR_H

class AffectsValidator : public ClauseValidator {
public:
    AffectsValidator(unordered_map<string, TokenType>* declarations, TokenType token);
    void validate(PqlToken* left, PqlToken* right);
};

#endif // TEAM37_AFFECTSVALIDATOR_H