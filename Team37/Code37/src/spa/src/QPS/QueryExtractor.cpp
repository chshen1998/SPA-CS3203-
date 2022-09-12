using namespace std;

#include <string>
#include <vector>

#include "QueryExtractor.h"
#include "QPS.h"

QueryExtractor::QueryExtractor(vector<PqlToken> &tokens) {
    next = tokens.begin();
    last = tokens.end();
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
    PqlToken patternSynonym = getNextToken();
    while (patternSynonym.type != TokenType::SYNONYM) {
        if (patternSynonym.type == TokenType::END) {
            return;
        }
        patternSynonym = getNextToken();
    }
    const PqlToken openBracket = getNextToken();
    const PqlToken synonym1 = getNextToken();

    if (synonym1.type != TokenType::VARIABLE) {
        throw "Error: Pattern clause parameters must be variable type";
    }
    pq.patternClause.left = synonym1.value;

    const PqlToken comma = getNextToken();
    const PqlToken synonym2 = getNextToken();

    if (synonym1.type != TokenType::VARIABLE) {
        throw "Error: Pattern clause parameters must be variable type";
    }
    pq.patternClause.right = synonym2.value;
    const PqlToken closedBracket = getNextToken();
}

void QueryExtractor::ExtractSuchThatClause() {
    PqlToken suchThatClause = getNextToken();
    while (!(validSuchThatClauses.find(suchThatClause.type) != validSuchThatClauses.end())) {
        if (suchThatClause.type == TokenType::END) {
            return;
        }
        suchThatClause = getNextToken();
    }
    const PqlToken openParenthesis = getNextToken();
    const PqlToken synonym1 = getNextToken();
    pq.suchThatClause.left = synonym1.value;

    const PqlToken comma = getNextToken();
    const PqlToken synonym2 = getNextToken();
    pq.suchThatClause.right = synonym2.value;

    const PqlToken closeParenthesis = getNextToken();
}

PqlToken QueryExtractor::getNextToken() {
    if (next != last)
    {
	    PqlToken token = *next;
        ++next;
        return token;
    }

    return PqlToken(TokenType::END, "");

}
