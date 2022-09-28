using namespace std;

#include <string>

#include "../Structures/PqlToken.h"
#include "../Types/TokenType.h"
#include "SelectTokenizer.h"
#include "TokenizerUtils.h"
#include <iostream>

using namespace TokenizerUtils;

void SelectTokenizer::tokenize() {
    TokenType currentToken;
    ClauseType type = ClauseType::NONE;
    int firstElement = true;

    while (currentIndex < delimited_query.size()) {
        /*
        The only valid symbols are "<", ">", ".", ","
        If it is not a synonym, checkTokenType will assign whatever token the string is,
        and validator will catch this error
        */ 
        currentToken = checkValidSymbol(delimited_query[currentIndex])
                       ? stringToTokenMap[delimited_query[currentIndex]]
                       : checkSelectTokenType(delimited_query[currentIndex], type);

        // Determining the return type for Select
        if (firstElement) {
            switch (currentToken) {
            case TokenType::OPEN_ARROW:
                type = ClauseType::TUPLE;
                break;
            case TokenType::BOOLEAN:
            case TokenType::SYNONYM:
                type = (currentIndex < delimited_query.size() - 1 && delimited_query[currentIndex + 1] == ".") 
                        ? ClauseType::ATTR_REF
                        : ClauseType::SINGLE;
                break;
            }
            firstElement = false;
        }
        
        tokens.push_back(PqlToken(currentToken, delimited_query[currentIndex]));
        currentIndex += 1;

        // For multi element Select return types
        if ((type == ClauseType::TUPLE && currentToken != TokenType::CLOSED_ARROW) ||
            (type == ClauseType::ATTR_REF && !checkIfAttrName(delimited_query[currentIndex - 1]))) {
            continue;
        }

        return;
    }
}


TokenType SelectTokenizer::checkSelectTokenType(const string& s, const ClauseType& clauseType) {
    if (clauseType == ClauseType::ATTR_REF
        && stringToTokenMap.find(s) != stringToTokenMap.end()) {
        return stringToTokenMap[s];
    }
    else if (clauseType == ClauseType::NONE && s == "BOOLEAN") {
        return TokenType::BOOLEAN;
    }
    else {
        return checkIfSynonym(s) ? TokenType::SYNONYM : TokenType::UNKNOWN;
    }
}
