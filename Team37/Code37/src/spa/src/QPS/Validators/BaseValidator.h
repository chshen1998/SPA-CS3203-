#pragma once

#ifndef TEAM37_BASEVALIDATOR_H
#define TEAM37_BASEVALIDATOR_H

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
#include "ValidatorUtils.h"

class BaseValidator {
protected:
    BaseValidator();

    PqlToken getNextToken();
    bool isDeclared(PqlToken* token);

    unordered_map<string, TokenType>* declarations;
    vector<PqlToken>* tokens;
    int next;
    int size;
};

#endif // TEAM37_BASEVALIDATOR_H