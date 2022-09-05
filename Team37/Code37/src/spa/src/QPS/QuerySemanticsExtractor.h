#pragma once

#include <string>
#include <vector>

#include "QPS.h"

#ifndef TEAM37_QUERYSEMANTICSEXTRACTOR_H
#define TEAM37_QUERYSEMANTICSEXTRACTOR_H

class QuerySemanticsExtractor {
public:
    vector<PqlToken>::iterator next;
    vector<PqlToken>::iterator end;
    PqlQuery pq;

    QuerySemanticsExtractor(vector<PqlToken> tokens);

    PqlQuery ExtractSemantics();

private:
    void ExtractDeclarations();

    void ExtractSelect();

    void ExtractClauses();

    void ExtractPatternClause();

    void ExtractSuchThatClause();

    PqlToken getNextToken();
};


#endif //TEAM37_QUERYSEMANTICSEXTRACTOR_H
