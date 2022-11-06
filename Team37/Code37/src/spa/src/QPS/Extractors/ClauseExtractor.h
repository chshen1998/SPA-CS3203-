#pragma once

#ifndef TEAM37_CLAUSEEXTRACTOR_H
#define TEAM37_CLAUSEEXTRACTOR_H

#include <memory>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

#include "../Structures/PqlError.h"
#include "../Structures/PqlQuery.h"
#include "../Structures/PqlToken.h"
#include "../Types/ErrorType.h"
#include "../Types/TokenType.h"
#include "./BaseExtractor.h"

class ClauseExtractor : public BaseExtractor {
public:
    ClauseExtractor(shared_ptr<PqlQuery> pq, vector<PqlToken>* tokens, bool booleanIsSynonym);
    void extract(int start, int last);

private:
    PqlToken extractPatternClause();

    PqlToken extractWithClause();

    PqlToken extractSuchThatClause();

    PqlToken extractString(PqlToken token);

    string removeWhitespace(string value);
};

#endif // TEAM37_CLAUSEEXTRACTOR_H