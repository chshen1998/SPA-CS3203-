#pragma once

#ifndef TEAM37_BASEEXTRACTOR_H
#define TEAM37_BASEEXTRACTOR_H

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


class BaseExtractor
{
public:
	virtual void extract(int start, int last) = 0;

protected:
	BaseExtractor(shared_ptr<PqlQuery> pq, vector<PqlToken>* tokens, bool booleanIsSynonym);

	PqlToken getNextToken();

	shared_ptr<PqlQuery> pq;
	vector<PqlToken>* tokens;
	int next;
	int end;
	bool booleanIsSynonym;
};

#endif //TEAM37_BASEEXTRACTOR_H