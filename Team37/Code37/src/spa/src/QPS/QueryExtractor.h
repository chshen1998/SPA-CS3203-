#pragma once

#include <string>
#include <vector>

#include "QPS.h"

#ifndef TEAM37_QUERYEXTRACTOR_H
#define TEAM37_QUERYEXTRACTOR_H

class QueryExtractor {
public:
    vector<PqlToken>::iterator next;
    vector<PqlToken>::iterator last;
    PqlQuery pq;

    QueryExtractor(vector<PqlToken> tokens);

    PqlQuery ExtractSemantics();

private:
    void ExtractDeclarations();

    void ExtractSelect();

    void ExtractClauses();

    void ExtractPatternClause();

    void ExtractSuchThatClause();

    PqlToken getNextToken();
};


#endif //TEAM37_QUERYEXTRACTOR_H
