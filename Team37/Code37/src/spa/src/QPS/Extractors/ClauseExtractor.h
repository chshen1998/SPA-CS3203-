#pragma once

#ifndef TEAM37_CLAUSEEXTRACTOR_H
#define TEAM37_CLAUSEEXTRACTOR_H

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


class ClauseExtractor : public BaseExtractor
{
public:
	ClauseExtractor(shared_ptr<PqlQuery> pq, vector<PqlToken>* tokens);
	void extract(int start, int last);

private:
	void extractPatternClause();

	void extractWithClause();

	void extractSuchThatClause();

	PqlToken extractString(PqlToken token);

	string removeWhitespace(string value);
};

#endif //TEAM37_CLAUSEEXTRACTOR_H