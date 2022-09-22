#pragma once

#ifndef TEAM37_QUERYEXTRACTOR_H
#define TEAM37_QUERYEXTRACTOR_H

#include <memory>
#include <string>
#include <vector>

#include "QPS.h"

class QueryExtractor {
public:
    vector<PqlToken> tokens;
    int size;
    int next;
    PqlQuery pq;

    QueryExtractor(vector<PqlToken> tokenVector);

    PqlQuery extractSemantics();

private:
    void extractDeclarations();
    
    void extractSelect();

    void extractClauses();

    void extractPatternClause();

    void extractSuchThatClause();

    PqlToken getNextToken();
};


#endif //TEAM37_QUERYEXTRACTOR_H
