using namespace std;

#include <string>
#include <vector>

#include "QueryExtractor.h"
#include "QPS.h"

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
        getNextToken();

        pq.declarations[synonym.value] = declaration.type;
        declaration = getNextToken();
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
    while (getNextToken().type != TokenType::OPEN_BRACKET) {}
    PqlToken left = getNextToken();
    getNextToken();
    PqlToken right = getNextToken();
    getNextToken();
    pq.patternClauses.push_back(Clause(left.value, right.value));
}

void QueryExtractor::extractSuchThatClause() {
    while (getNextToken().type != TokenType::OPEN_BRACKET) {}
    PqlToken left = getNextToken();
    getNextToken();
    PqlToken right = getNextToken();
    getNextToken();
    pq.suchThatClauses.push_back(Clause(left.value, right.value));
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
