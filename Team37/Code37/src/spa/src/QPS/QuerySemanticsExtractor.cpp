using namespace std;

#include <string>
#include <vector>

#include "QuerySemanticsExtractor.h"
#include "Parser.h"

QuerySemanticsExtractor::QuerySemanticsExtractor(vector<PqlToken> &tokens) {
    next = tokens.begin();
    end = tokens.end();
    pq = PqlQuery();
}

PqlQuery QuerySemanticsExtractor::ExtractSemantics() {
    ExtractDeclarations();
    ExtractSelect();
    return pq;
}

void QuerySemanticsExtractor::ExtractDeclarations() {
    while (true) {
        const PqlToken declaration = getNextToken();
        if (declaration.type == TokenType::WHITESPACE) {
            // Reached end of declarations
            return;
        }
        const PqlToken synonym = getNextToken();
        const PqlToken semicolon = getNextToken();

        pq.declarations[synonym.value] = declaration.type;
    }

}

void QuerySemanticsExtractor::ExtractSelect() {
    const PqlToken selectClause = getNextToken();
    if (selectClause.type != TokenType::SELECT) {
        throw "Error: No select clause";
    }
    const PqlToken synonym = getNextToken();

    pq.select = synonym.value;
}

void QuerySemanticsExtractor::ExtractClauses() {
    const PqlToken nextToken = getNextToken();
    if (nextToken.type == TokenType::END) {
        return;
    }

    if (nextToken.type == TokenType::PATTERN) {
        ExtractPatternClause();
        ExtractSuchThatClause();
    } else {
        ExtractSuchThatClause();
        ExtractPatternClause();
    }
}

void QuerySemanticsExtractor::ExtractPatternClause() {
    PqlToken patternSynonym = getNextToken();
    while (patternSynonym.type != TokenType::SYNONYM) {
        patternSynonym = getNextToken();
    }
    const PqlToken openBracket = getNextToken();
    const PqlToken synonym1 = getNextToken();
    const PqlToken comma = getNextToken();
    const PqlToken synonym2 = getNextToken();
    const PqlToken closedBracket = getNextToken();
}

void QuerySemanticsExtractor::ExtractSuchThatClause() {
    PqlToken suchThatClause = getNextToken();
    while (!(validSuchThatClauses.find(suchThatClause.type) != validSuchThatClauses.end())) {
        suchThatClause = getNextToken();
    }
    const PqlToken openParenthesis = getNextToken();
    const PqlToken synonym1 = getNextToken();
    const PqlToken comma = getNextToken();
    const PqlToken synonym2 = getNextToken();
    const PqlToken closeParenthesis = getNextToken();
}

PqlToken QuerySemanticsExtractor::getNextToken() {
    if (next == end) {
        return PqlToken(TokenType::END, "");
    } else {
        const PqlToken token = *next;
        next++;
        return token;
    }
}
