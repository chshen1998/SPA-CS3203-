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
            extractPatternClause();
	    }
        else
        {
            extractSuchThatClause();
        }
        nextToken = getNextToken();
    }
}

void QueryExtractor::extractPatternClause() {
    PqlToken synonym = getNextToken();
    getNextToken(); // OPEN BRACKET
    //while (getNextToken().type != TokenType::OPEN_BRACKET) {}
    PqlToken left = getNextToken();
    getNextToken();
    PqlToken right = getNextToken();
    getNextToken();
    pq.patternClauses.push_back(Clause(synonym, left, right));
}

void QueryExtractor::extractSuchThatClause() {
    getNextToken(); // THAT
    PqlToken suchThatClause = getNextToken();
    getNextToken(); // OPEN BRACKET
    //while (getNextToken().type != TokenType::OPEN_BRACKET) {}
    PqlToken left = getNextToken();
    getNextToken();
    PqlToken right = getNextToken();
    getNextToken();
    pq.suchThatClauses.push_back(Clause(suchThatClause, left, right));
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
