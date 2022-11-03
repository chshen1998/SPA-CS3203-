#pragma once

#ifndef TEAM37_DECLARATIONEXTRACTOR_H
#define TEAM37_DECLARATIONEXTRACTOR_H

#include <string>
#include <vector>
#include <unordered_map>
#include <set>
#include <memory>

#include "../Structures/PqlError.h"
#include "../Structures/PqlToken.h"
#include "../Structures/PqlQuery.h"
#include "../Types/ErrorType.h"
#include "../Types/TokenType.h"
#include "./BaseExtractor.h"


class DeclarationExtractor : public BaseExtractor
{
public:
	DeclarationExtractor(shared_ptr<PqlQuery> pq, vector<PqlToken>* tokens);
	void extract(int start, int last);
};

#endif //TEAM37_DECLARATIONEXTRACTOR_H