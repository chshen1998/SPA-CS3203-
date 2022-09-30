using namespace std;

#include <string>

#include "../Structures/PqlToken.h"
#include "../Types/TokenType.h"
#include "WithTokenizer.h"
#include "TokenizerUtils.h"
#include <iostream>

using namespace TokenizerUtils;

void WithTokenizer::tokenize() {
    TokenType currentToken;
    bool secondRef = false;

    while (currentIndex < delimited_query.size()) {

        // Can only be attrName, dot, equal
        currentToken = (stringToTokenMap.find(delimited_query[currentIndex]) != stringToTokenMap.end())
            ? stringToTokenMap[delimited_query[currentIndex]]
            : checkWithTokenType(delimited_query[currentIndex]);

        secondRef = currentToken == TokenType::EQUAL;

        tokens.push_back(PqlToken(currentToken, delimited_query[currentIndex]));
        currentIndex += 1;

        if (secondRef && checkIfLastElement(delimited_query[currentIndex - 1])) {
            if (currentIndex < delimited_query.size() && delimited_query[currentIndex] != "and") {
                tokens.push_back(PqlToken(TokenType::AND, delimited_query[currentIndex]));
                currentIndex += 1;
                secondRef = false;
            }
            else {
                return;
            }
        }
    }
}

inline bool WithTokenizer::checkIfLastElement(const string& s) {
    return checkIfString(s) || checkIfAttrName(s) || checkIfInteger(s);
}

TokenType WithTokenizer::checkWithTokenType(const string& s) {
    return checkIfSynonym(s) ? TokenType::SYNONYM
        : checkIfString(s) ? TokenType::STRING
        : checkIfInteger(s) ? TokenType::NUMBER
        : TokenType::UNKNOWN;
}



