#include <iso646.h>
using namespace std;

#include <string>
#include <vector>

#include "QueryExtractor.h"
#include "./Structures/PqlError.h"
#include "./Structures/PqlToken.h"
#include "./Structures/PqlQuery.h"
#include "./Types/ErrorType.h"
#include "./Types/TokenType.h"

QueryExtractor::QueryExtractor(vector<PqlToken> tokenVector)
{
    tokens = tokenVector;
    size = tokens.size();
    next = 0;
    pq = PqlQuery();
 
}

PqlQuery QueryExtractor::extractSemantics()
{
    extractDeclarations();
    extractSelect();
    extractClauses();
    return pq;
}

void QueryExtractor::extractDeclarations()
{
    PqlToken declaration = getNextToken();
    while (declaration.type != TokenType::DECLARATION_END) {
        PqlToken synonym = getNextToken();
        PqlToken symbol = getNextToken();
        pq.declarations[synonym.value] = declaration.type;

        if (symbol.type == TokenType::SEMICOLON) {
            declaration = getNextToken();
        }
    }
}

void QueryExtractor::extractSelect()
{
    getNextToken();
    const PqlToken synonym = getNextToken();
    pq.select = synonym.value;
}

void QueryExtractor::extractClauses()
{
    PqlToken nextToken = getNextToken();

    while (nextToken.type != TokenType::END)
    {
	    if (nextToken.type == TokenType::PATTERN)
	    {
            nextToken = extractPatternClause();
	    }
        else if (nextToken.type == TokenType::WITH)
        {
            nextToken = extractWithClause();
        }
        else
        {
            nextToken = extractSuchThatClause();
        }
    }
}

PqlToken QueryExtractor::extractPatternClause()
{
    PqlToken pattern;
    PqlToken left;
    PqlToken right;

    PqlToken next = PqlToken(TokenType::AND, "and");
    while (next.type == TokenType::AND)
    {
        pattern = getNextToken();
        getNextToken(); // OPEN BRACKET
        left = getNextToken();
        getNextToken(); // COMMA
        right = getNextToken();
        getNextToken(); // CLOSE BRACKET
        pq.clauses.push_back(Clause(pattern, left, right, TokenType::PATTERN));
        next = getNextToken();
    }
    return next;
}

PqlToken QueryExtractor::extractWithClause()
{
    PqlToken left;
    PqlToken leftAttr;
    PqlToken right;
    PqlToken rightAttr;

    PqlToken next = PqlToken(TokenType::AND, "and");
    while (next.type == TokenType::AND)
    {
        left = getNextToken();
        leftAttr = PqlToken(TokenType::NONE, "");

        next = getNextToken(); // Either "." or "="
        if (next.type == TokenType::DOT)
        {
            leftAttr = getNextToken();
            getNextToken(); // "="
        }

        right = getNextToken();
        rightAttr = PqlToken(TokenType::NONE, "");

        next = getNextToken(); // Either "." or "and"
        if (next.type == TokenType::DOT)
        {
            rightAttr = getNextToken();
            next = getNextToken(); // Either "and" or next clause type
        }

        pq.clauses.push_back(Clause(PqlToken(TokenType::NONE, ""), left, right, TokenType::WITH, leftAttr, rightAttr));
    }

    return next;
}


PqlToken QueryExtractor::extractSuchThatClause()
{
	getNextToken(); // THAT

    PqlToken suchThatClause;
    PqlToken left;
    PqlToken right;

    PqlToken next = PqlToken(TokenType::AND, "and");
    while (next.type == TokenType::AND) 
    {
	    suchThatClause = getNextToken();
	    getNextToken(); // OPEN BRACKET
	    left = getNextToken();
	    getNextToken(); // COMMA
	    right = getNextToken();
	    getNextToken(); // CLOSE BRACKET
	    pq.clauses.push_back(Clause(suchThatClause, left, right, TokenType::SUCH_THAT));
        next = getNextToken();
    }

    return next;
}

PqlToken QueryExtractor::getNextToken()
{
    if (next == size)
    {
        return PqlToken(TokenType::END, "");
    }
    PqlToken token = tokens[next];
    next = next +1;
    return token;

}
