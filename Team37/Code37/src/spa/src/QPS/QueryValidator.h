#pragma once

#ifndef TEAM37_QUERYVALIDATOR_H
#define TEAM37_QUERYVALIDATOR_H

#include <memory>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

#include "./Structures/PqlError.h"
#include "./Structures/PqlQuery.h"
#include "./Structures/PqlToken.h"
#include "./Types/ErrorType.h"
#include "./Types/TokenType.h"
#include "Validators/ClauseValidator.h"
#include "Validators/PatternValidator.h"
#include "Validators/WithValidator.h"

class QueryValidator {
public:
    vector<PqlToken>* tokens;
    int size;
    int next;
    bool booleanIsSynonym;
    unordered_map<string, TokenType> declarations;

    QueryValidator(vector<PqlToken>* tokenVector);

    PqlError validateQuery();

private:
    void validateDeclarations();
    void validateSelect();
    void validateClauses();
    PqlToken validatePattern();
    PqlToken validateWith();
    PqlToken validateSuchThat();

    shared_ptr<ClauseValidator> createClauseValidator(TokenType type);
    PqlToken getNextToken();
};

#endif // TEAM37_QUERYVALIDATOR_H
