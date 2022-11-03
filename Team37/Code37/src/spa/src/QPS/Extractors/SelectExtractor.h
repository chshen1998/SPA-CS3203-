#pragma once

#ifndef TEAM37_SELECTEXTRACTOR_H
#define TEAM37_SELECTEXTRACTOR_H

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


class SelectExtractor : public BaseExtractor
{
public:
	SelectExtractor(shared_ptr<PqlQuery> pq, vector<PqlToken>* tokens);
	void extract(int start, int last);

private:
	PqlToken extractSelectObject(PqlToken curr);
};

#endif //TEAM37_SELECTEXTRACTOR_H