#pragma once

#ifndef TEAM37_PATTERNVALIDATOR_H
#define TEAM37_PATTERNVALIDATOR_H

#include <memory>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

#include "../Structures/PqlError.h"
#include "../Structures/PqlQuery.h"
#include "../Structures/PqlToken.h"
#include "../Types/ErrorType.h"
#include "../Types/TokenType.h"
#include "ClauseValidator.h"

class PatternValidator : public ClauseValidator {
public:
    PatternValidator(unordered_map<string, TokenType>* declarations);
    void validatePattern(PqlToken* assign);
    void validate(PqlToken* left, PqlToken* right);
    void validateWhile(PqlToken* left, PqlToken* right);
    void validateIf(PqlToken* eft, PqlToken* mid, PqlToken* right);

private:
    void validateExpressionSpec(PqlToken* token);
    void validateWildcard(PqlToken* token);
};

#endif // TEAM37_PATTERNVALIDATOR_H
