#pragma once

#ifndef TEAM37_WITHVALIDATOR_H
#define TEAM37_WITHVALIDATOR_H

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
#include "BaseValidator.h"
#include "ClauseValidator.h"

class WithValidator : public BaseValidator {
public:
    WithValidator(unordered_map<string, TokenType>* declarationsMap, vector<PqlToken>* withTokens);
    void validate();

private:
    TokenType validateRef(vector<PqlToken> refTokens);
};

#endif // TEAM37_WITHVALIDATOR_H