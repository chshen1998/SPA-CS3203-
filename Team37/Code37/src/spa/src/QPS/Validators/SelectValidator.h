#pragma once

#ifndef TEAM37_SELECTVALIDATOR_H
#define TEAM37_SELECTVALIDATOR_H

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

class SelectValidator : public BaseValidator {
public:
    SelectValidator(unordered_map<string, TokenType>* declarations);
    void validateSelect(TokenType type);
    void validateSynonym(PqlToken token);
    void validateAttrName(PqlToken dot, PqlToken attrName);
    void validateMultiple(vector<PqlToken>* tokens);

private:
    void validateSingle(int start, int end);
    void validateAttrNameMatch(PqlToken synonym, PqlToken attrName);
};

#endif // TEAM37_SELECTVALIDATOR_H