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

BaseExtractor::BaseExtractor(shared_ptr<PqlQuery> pq_ptr, vector<PqlToken>* tokenVector, bool boolean) {
	pq = pq_ptr;
	tokens = tokenVector;
    booleanIsSynonym = boolean;
}

PqlToken BaseExtractor::getNextToken()
{
    if (next == end)
    {
        return PqlToken(TokenType::END, "");
    }
    PqlToken token = tokens->at(next);
    next = next + 1;

    if (token.type == TokenType::BOOLEAN && booleanIsSynonym == true) {
        token.type = TokenType::SYNONYM;
    }
    return token;
}