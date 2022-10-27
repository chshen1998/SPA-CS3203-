#pragma once

#ifndef TEAM37_QUERYEXTRACTOR_H
#define TEAM37_QUERYEXTRACTOR_H

#include <memory>
#include <string>
#include <vector>

#include "./Structures/PqlError.h"
#include "./Structures/PqlToken.h"
#include "./Structures/PqlQuery.h"
#include "./Types/ErrorType.h"
#include "./Types/TokenType.h"

extern set<char> validOperators;

class QueryExtractor {
public:
    vector<PqlToken> *tokens;
    int size;
    int next;
    bool booleanIsSynonym;
    shared_ptr<PqlQuery> pq;

    QueryExtractor(vector<PqlToken> *tokenVector, shared_ptr<PqlQuery> pq);

    void extractSemantics();

private:
    void extractDeclarations();
    
    PqlToken extractSelect();

    PqlToken extractSelectObject(PqlToken curr);

    void extractClauses(PqlToken curr);

    PqlToken extractPatternClause();

    PqlToken extractWithClause();

    PqlToken extractSuchThatClause();

    PqlToken extractString(PqlToken token);

    PqlToken getNextToken();
};


#endif //TEAM37_QUERYEXTRACTOR_H
