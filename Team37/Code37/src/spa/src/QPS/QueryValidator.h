#pragma once

#include <memory>
#include <string>
#include <set>
#include <vector>
#include <unordered_map>

#include "QPS.h"
#include "Validators/ClauseValidator.h"

#ifndef TEAM37_QUERYVALIDATOR_H
#define TEAM37_QUERYVALIDATOR_H


class QueryValidator {
public:
    vector<PqlToken>::iterator next;
    vector<PqlToken>::iterator end;
    unordered_map<string, TokenType> declarations;
    PqlError pe;

    QueryValidator(vector<PqlToken> &tokens);

    PqlError ValidateQuery();

private:
    void validateDeclarations();
    void validateSelect();
    void validateClauses();
    void validatePattern();
    void validateSuchThat(PqlToken such);
    bool errorFound();

    unique_ptr<ClauseValidator> createClauseValidator(TokenType type);
    PqlToken getNextToken();

};


#endif //TEAM37_QUERYVALIDATOR_H
