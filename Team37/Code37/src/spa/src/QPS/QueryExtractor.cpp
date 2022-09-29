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

QueryExtractor::QueryExtractor(vector<PqlToken> tokenVector) {
    tokens = tokenVector;
    size = tokens.size();
    next = 0;
    pq = PqlQuery();
 
}

PqlQuery QueryExtractor::extractSemantics() {
    extractDeclarations();
    extractSelect();
    extractClauses();
    return pq;
}

void QueryExtractor::extractDeclarations() {
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

void QueryExtractor::extractSelect() {
    getNextToken();
    const PqlToken synonym = getNextToken();
    pq.select = synonym.value;
}

void QueryExtractor::extractClauses() {
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

PqlToken QueryExtractor::extractPatternClause() {
    PqlToken next = PqlToken(TokenType::AND, "and");
    while (next.type == TokenType::AND)
    {
        PqlToken synonym = getNextToken();
        getNextToken(); // OPEN BRACKET
        PqlToken left = getNextToken();
        getNextToken(); // COMMA
        PqlToken right = getNextToken();
        getNextToken(); // CLOSE BRACKET
        pq.clauses.push_back(PatternClause(synonym, left, right));
        next = getNextToken();
    }
    return next;
}

PqlToken QueryExtractor::extractWithClause()
{
    PqlToken next = PqlToken(TokenType::AND, "and");
    while (next.type == TokenType::AND)
    {
	    
    }

    return next;
}


PqlToken QueryExtractor::extractSuchThatClause() {
    getNextToken(); // THAT

    PqlToken next = PqlToken(TokenType::AND, "and");
    while (next.type == TokenType::AND) 
    {
	    PqlToken suchThatClause = getNextToken();
	    getNextToken(); // OPEN BRACKET
	    PqlToken left = getNextToken();
	    getNextToken(); // COMMA
	    PqlToken right = getNextToken();
	    getNextToken(); // CLOSE BRACKET
	    pq.clauses.push_back(SuchThatClause(suchThatClause, left, right));
        next = getNextToken();
    }

    return next;
}

PqlToken QueryExtractor::getNextToken() {
    if (next == size)
    {
        return PqlToken(TokenType::END, "");
    }
    PqlToken token = tokens[next];
    next = next +1;
    return token;

}
