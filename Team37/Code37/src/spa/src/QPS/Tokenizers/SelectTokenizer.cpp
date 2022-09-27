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
    SelectType type = SelectType::UNKNOWN;
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

        // Determining the Select Type
        if (firstElement) {
            switch (currentToken) {
            case TokenType::OPEN_ARROW:
                type = SelectType::TUPLE;
                break;
            case TokenType::BOOLEAN:
            case TokenType::SYNONYM:
                type = (currentIndex < delimited_query.size() - 1 && delimited_query[currentIndex + 1] == ".") 
                        ? SelectType::ATTR_REF
                        : SelectType::SINGLE;
                break;
            }
            firstElement = false;
        }
        
        tokens.push_back(PqlToken(currentToken, delimited_query[currentIndex]));
        currentIndex += 1;

        // For multi element Select return types
        if ((type == SelectType::TUPLE && currentToken != TokenType::CLOSED_ARROW) ||
            (type == SelectType::ATTR_REF && !checkIfAttrName(delimited_query[currentIndex - 1]))) {
            continue;
        }

        return;
    }
}


TokenType SelectTokenizer::checkSelectTokenType(const string& s, const SelectType& clauseType) {
    if (clauseType == SelectType::ATTR_REF
        && stringToTokenMap.find(s) != stringToTokenMap.end()) {
        return stringToTokenMap[s];
    }
    else if (clauseType == SelectType::UNKNOWN && s == "BOOLEAN") {
        return TokenType::BOOLEAN;
    }
    else {
        return checkIfSynonym(s) ? TokenType::SYNONYM : TokenType::UNKNOWN;
    }
}
