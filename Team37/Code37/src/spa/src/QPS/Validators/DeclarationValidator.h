#pragma once

#ifndef TEAM37_DECLARATIONVALIDATOR_H
#define TEAM37_DECLARATIONVALIDATOR_H

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

class DeclarationValidator : public BaseValidator {
public:
    DeclarationValidator(vector<PqlToken>* tokens, unordered_map<string, TokenType>* map);
    void validate();

private:
    void isValidDesignEntity(PqlToken* token);
    void isValidSynonym(PqlToken* token);
    void isSemicolonOrComma(PqlToken* token);
};

#endif // TEAM37_DECLARATIONVALIDATOR_H
