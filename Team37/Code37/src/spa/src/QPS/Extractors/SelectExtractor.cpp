#include <iso646.h>

using namespace std;

#include <memory>
#include <set>
#include <string>
#include <vector>

#include ".././Structures/PqlError.h"
#include ".././Structures/PqlQuery.h"
#include ".././Structures/PqlToken.h"
#include ".././Types/TokenType.h"
#include "BaseExtractor.h"
#include "SelectExtractor.h"

SelectExtractor::SelectExtractor(shared_ptr<PqlQuery> pq_ptr, vector<PqlToken>* tokenVector, bool boolean)
    : BaseExtractor(pq_ptr, tokenVector, boolean)
{
}

void SelectExtractor::extract(int start, int last)
{
    next = start;
    end = last;

    getNextToken(); // Select token

    PqlToken token = getNextToken();
    if (token.type == TokenType::OPEN_ARROW) {
        PqlToken comma = PqlToken(TokenType::COMMA, ",");

        // While loop ends when closed arrow is reached
        while (comma.type == TokenType::COMMA) {
            token = getNextToken(); // Get token after open arrow/comma
            comma = extractSelectObject(token); // Returns either comma or closed arrow
        }
    } else {
        token = extractSelectObject(token);
    }
}

PqlToken SelectExtractor::extractSelectObject(PqlToken curr)
{
    PqlToken nextToken = getNextToken();
    if (nextToken.type == TokenType::DOT) {
        nextToken = getNextToken();
        pq->selectObjects.push_back(shared_ptr<SelectObject>(new SelectObject(SelectType::ATTRNAME, curr.value, nextToken)));
        nextToken = getNextToken();
    } else if (curr.type == TokenType::BOOLEAN) {
        pq->selectObjects.push_back(shared_ptr<SelectObject>(new SelectObject(SelectType::BOOLEAN)));
    } else {
        pq->selectObjects.push_back(shared_ptr<SelectObject>(new SelectObject(SelectType::SYNONYM, curr.value)));
    }

    return nextToken;
}
