#pragma once

#ifndef TEAM37_QUERYVALIDATOR_H
#define TEAM37_QUERYVALIDATOR_H

#include <memory>
#include <string>
#include <set>
#include <vector>
#include <unordered_map>

#include "./Structures/PqlError.h"
#include "./Structures/PqlToken.h"
#include "./Structures/PqlQuery.h"
#include "./Types/ErrorType.h"
#include "./Types/TokenType.h"
#include "Validators/ClauseValidator.h"


class QueryValidator {
public:
    vector<PqlToken> tokens;
    int size;
    int next;
    unordered_map<string, TokenType> declarations;

    QueryValidator(vector<PqlToken> tokens);

    PqlError validateQuery();

private:
    unordered_map<string, TokenType> validateDeclarations();
    void validateSelect();
    void validateClauses();
    void validatePattern();
    void validateSuchThat(PqlToken such);

    shared_ptr<ClauseValidator> createClauseValidator(TokenType type);
    PqlToken getNextToken();

};


#endif //TEAM37_QUERYVALIDATOR_H
