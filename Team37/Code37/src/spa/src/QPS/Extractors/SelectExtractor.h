#pragma once

#ifndef TEAM37_SELECTEXTRACTOR_H
#define TEAM37_SELECTEXTRACTOR_H

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

class SelectExtractor : public BaseExtractor {
public:
    SelectExtractor(shared_ptr<PqlQuery> pq, vector<PqlToken>* tokens, bool booleanIsSynonym);
    void extract(int start, int last);

private:
    PqlToken extractSelectObject(PqlToken curr);
};

#endif // TEAM37_SELECTEXTRACTOR_H