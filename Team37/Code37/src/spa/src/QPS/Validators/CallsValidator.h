#pragma once

#ifndef TEAM37_CALLSVALIDATOR_H
#define TEAM37_CALLSVALIDATOR_H

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

class CallsValidator : public ClauseValidator {
public:
    CallsValidator(unordered_map<string, TokenType>* declarations, TokenType token);
    void validate(PqlToken* left, PqlToken* right);
};

#endif // TEAM37_CALLSVALIDATOR_H