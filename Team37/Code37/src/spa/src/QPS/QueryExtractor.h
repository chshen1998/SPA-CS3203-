#pragma once

#ifndef TEAM37_QUERYEXTRACTOR_H
#define TEAM37_QUERYEXTRACTOR_H

#include <memory>
#include <string>
#include <vector>

#include "./Structures/PqlError.h"
#include "./Structures/PqlQuery.h"
#include "./Structures/PqlToken.h"
#include "./Types/ErrorType.h"
#include "./Types/TokenType.h"

class QueryExtractor {
public:
    QueryExtractor(vector<PqlToken>* tokenVector, shared_ptr<PqlQuery> pq_pointer);

    void extractSemantics();

    vector<PqlToken>* tokens;
    int next;
    int size;
    bool booleanIsSynonym;
    shared_ptr<PqlQuery> pq;

private:
    void extractDeclarations();

    void extractSelect();

    void extractClauses();

    void checkBooleanSynonym();
};

#endif // TEAM37_QUERYEXTRACTOR_H
