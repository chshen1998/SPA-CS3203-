#include <iso646.h>

using namespace std;

#include <set>
#include <string>
#include <vector>

#include ".././Structures/PqlError.h"
#include ".././Structures/PqlQuery.h"
#include ".././Structures/PqlToken.h"
#include ".././Types/TokenType.h"
#include "BaseExtractor.h"
#include "DeclarationExtractor.h"

DeclarationExtractor::DeclarationExtractor(shared_ptr<PqlQuery> pq_ptr, vector<PqlToken>* tokenVector)
    : BaseExtractor(pq_ptr, tokenVector, false)
{
}

void DeclarationExtractor::extract(int start, int last)
{
    next = start;
    end = last;

    PqlToken declaration = getNextToken();
    while (declaration.type != TokenType::END) {
        PqlToken synonym = getNextToken();
        PqlToken symbol = getNextToken();
        pq->declarations[synonym.value] = declaration.type;

        if (symbol.type == TokenType::SEMICOLON) {
            declaration = getNextToken();
        }
    }
}