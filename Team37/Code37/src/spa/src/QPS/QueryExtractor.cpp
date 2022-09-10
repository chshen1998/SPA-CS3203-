using namespace std;

#include <string>
#include <vector>

#include "QueryExtractor.h"
#include "QPS.h"

QueryExtractor::QueryExtractor(vector<PqlToken> &tokens) {
    next = begin(tokens);
    last = end(tokens);
    pq = PqlQuery();
}

PqlQuery QueryExtractor::ExtractSemantics() {
    ExtractDeclarations();
    ExtractSelect();
    ExtractClauses();
    return pq;
}

void QueryExtractor::ExtractDeclarations() {
    PqlToken declaration = getNextToken();
    while (declaration.type != TokenType::DECLARATION_END) {
        PqlToken synonym = getNextToken();
        getNextToken();

        pq.declarations[synonym.value] = declaration.type;
        declaration = getNextToken();
    }
}

void QueryExtractor::ExtractSelect() {
    getNextToken();
    const PqlToken synonym = getNextToken();
    pq.select = synonym.value;
}

void QueryExtractor::ExtractClauses() {
    const PqlToken nextToken = getNextToken();
    if (nextToken.type == TokenType::END) {
        return;
    }

    if (nextToken.type == TokenType::PATTERN) {
        ExtractPatternClause();

        if (nextToken.type != TokenType::END) {
			ExtractSuchThatClause();
        }
        
    } else {
        ExtractSuchThatClause();

        if (nextToken.type != TokenType::END) {
            ExtractPatternClause();
        }
    }
}

void QueryExtractor::ExtractPatternClause() {
}

void QueryExtractor::ExtractSuchThatClause() {
}

PqlToken QueryExtractor::getNextToken() {
    if (next == last) {
        return PqlToken(TokenType::END, "");
    } else {
        PqlToken token = *next;
        ++next;
        return token;
    }
}
