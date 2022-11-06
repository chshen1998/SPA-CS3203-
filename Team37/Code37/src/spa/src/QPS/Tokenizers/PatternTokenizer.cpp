using namespace std;

#include <string>

#include "../Structures/PqlToken.h"
#include "../Types/TokenType.h"
#include "PatternTokenizer.h"
#include "TokenizerUtils.h"
#include <iostream>

using namespace TokenizerUtils;

void PatternTokenizer::tokenize() {
    TokenType currentToken;
    int clauseCounter = 1;

    while (currentIndex < delimited_query.size()) {
        currentToken = TokenType::UNKNOWN;

        if (clauseCounter % 2 != 0) {
            currentToken = checkPatternTokenType(delimited_query[currentIndex], clauseCounter);
        }
        else {
            if (stringToTokenMap.find(delimited_query[currentIndex]) != stringToTokenMap.end()) {
                currentToken = stringToTokenMap[delimited_query[currentIndex]];
            }
        }

        tokens.push_back(PqlToken(currentToken, delimited_query[currentIndex]));
        currentIndex += 1;
        clauseCounter += 1;

        if (currentToken == TokenType::CLOSED_BRACKET) {
            if (currentIndex < delimited_query.size() && delimited_query[currentIndex] == "and") {
                tokens.push_back(PqlToken(TokenType::AND, delimited_query[currentIndex]));
                currentIndex += 1;
                clauseCounter = patternClauseFirstArgIndex;
            }
            else {
                return;
            }
        }
    }
}


TokenType PatternTokenizer::checkPatternTokenType(const string & s, int argNum) {
    switch (argNum) {
    case patternClauseFirstArgIndex:
        return checkIfSynonym(s) ? TokenType::SYNONYM : TokenType::UNKNOWN;
    case patternClauseSecondArgIndex:
        return getEntRefToken(s);
    case patternClauseThirdArgIndex:
        return getExpressionSpec(s);
    case patternClauseFourthArgIndex:
        return s == "_" ? TokenType::WILDCARD : TokenType::UNKNOWN;
    default:
        return TokenType::UNKNOWN;
    }
}
  

