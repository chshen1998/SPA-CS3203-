#include <iostream>
#include <iso646.h>

using namespace std;

#include <string>
#include <vector>
#include <set>

#include ".././Structures/PqlError.h"
#include ".././Structures/PqlToken.h"
#include ".././Structures/PqlQuery.h"
#include ".././Types/TokenType.h"
#include "BaseExtractor.h"

BaseExtractor::BaseExtractor(shared_ptr<PqlQuery> pq_ptr, vector<PqlToken>* tokenVector) {
	pq = pq_ptr;
	tokens = tokenVector;
}

PqlToken BaseExtractor::getNextToken()
{
    if (next == end)
    {
        return PqlToken(TokenType::END, "");
    }
    PqlToken token = tokens->at(next);
    cout << token.value;
    next = next + 1;
    return token;
}