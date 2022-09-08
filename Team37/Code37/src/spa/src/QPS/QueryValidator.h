#pragma once

#include <string>
#include <set>
#include <vector>

#include "QPS.h"

#ifndef TEAM37_QUERYVALIDATOR_H
#define TEAM37_QUERYVALIDATOR_H


class QueryValidator {
public:
    vector<PqlToken>::iterator next;
    vector<PqlToken>::iterator end;
    bool hasSelectClause;
    bool hasDeclarations;
    PqlError pe;

    QueryValidator(vector<PqlToken> &tokens);

    PqlError ValidateQuery();

private:
    PqlToken validateDeclarations();
    void validateSelectClause(PqlToken select);
    bool isValidDeclarationType(TokenType type);
    bool errorFound();
    PqlToken getNextToken();

};


#endif //TEAM37_QUERYVALIDATOR_H
