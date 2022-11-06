#pragma once

#ifndef TEAM37_CLAUSEVALIDATOR_H
#define TEAM37_CLAUSEVALIDATOR_H

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
#include "ValidatorUtils.h"

class ClauseValidator : public BaseValidator {
public:
    void validateOpen(TokenType type);
    void validateClose(TokenType type);
    void validateComma(TokenType type);
    virtual void validate(PqlToken* left, PqlToken* right) = 0;

protected:
    ClauseValidator(unordered_map<string, TokenType>* declarationsMap, TokenType token);

    void validateEntityRef(PqlToken* token, TokenType entityType);
    void validateStatementRef(PqlToken* token);

    TokenType validatorType;
};

#endif // TEAM37_CLAUSEVALIDATOR_H
