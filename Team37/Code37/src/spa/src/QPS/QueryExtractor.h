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

class QueryExtractor {
public:
    vector<PqlToken> *tokens;
    int next;
    shared_ptr<PqlQuery> pq;

    QueryExtractor(vector<PqlToken> *tokenVector, shared_ptr<PqlQuery> pq);

    void extractSemantics();

private:
    void extractDeclarations();
    
    void extractSelect();

    void extractClauses();
};


#endif //TEAM37_QUERYEXTRACTOR_H
