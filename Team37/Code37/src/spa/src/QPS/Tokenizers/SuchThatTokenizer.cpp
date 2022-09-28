using namespace std;

#include <string>

#include "../Structures/PqlToken.h"
#include "../Types/TokenType.h"
#include "SuchThatTokenizer.h"
#include "TokenizerUtils.h"
#include <iostream>

using namespace TokenizerUtils;

void SuchThatTokenizer::tokenize() {
    TokenType currentToken;
    ClauseType type = ClauseType::NONE;
    int clauseCounter = 1;

    while (currentIndex < delimited_query.size()) {
        currentToken = TokenType::UNKNOWN;

        if (clauseCounter == suchThatClauseFirstArgIndex ||
            clauseCounter == suchThatClauseSecondArgIndex) {
            currentToken = checkSuchThatTokenType(delimited_query[currentIndex], type, clauseCounter);
        }
        else {
            if (stringToTokenMap.find(delimited_query[currentIndex]) != stringToTokenMap.end()) {
                currentToken = stringToTokenMap[delimited_query[currentIndex]];

                if (clauseCounter == suchThatClauseTypeIndex) {
                    type = tokenTypeToClauseTypeMap[currentToken];
                }
            }
        }

        tokens.push_back(PqlToken(currentToken, delimited_query[currentIndex]));
        currentIndex += 1;
        clauseCounter += 1;

        if (currentToken == TokenType::CLOSED_BRACKET) {
            if (currentIndex < delimited_query.size() && delimited_query[currentIndex] == "and") {
                tokens.push_back(PqlToken(TokenType::AND, delimited_query[currentIndex]));
                currentIndex += 1;
                clauseCounter = suchThatClauseTypeIndex;
            }
            else {
                return;
            }
        }
    }
}


TokenType SuchThatTokenizer::checkSuchThatTokenType(const string& s, const ClauseType& clauseType, const int& argNum) {
    switch (clauseType) {
    case ClauseType::USE:
    case ClauseType::MODIFIES:
        return argNum == suchThatClauseFirstArgIndex ? getStmtRefToken(s) : getEntRefToken(s);
    case ClauseType::CALL:
        return getEntRefToken(s);
    default:
        return getStmtRefToken(s);
    }
}
