using namespace std;

#include <string>

#include "../Structures/PqlToken.h"
#include "../Types/TokenType.h"
#include "SelectTokenizer.h"
#include "TokenizerUtils.h"
#include <iostream>

using namespace TokenizerUtils;

void SelectTokenizer::tokenize() {

    // Has to be boolean, synonym or attrRef (all single values)
    if (delimited_query[currentIndex] != "<") {
        // Boolean 
        if (delimited_query[currentIndex] == "BOOLEAN") {
            tokens.push_back(PqlToken(TokenType::BOOLEAN, delimited_query[currentIndex]));
        }

        else if (checkIfSynonym(delimited_query[currentIndex])) {
            
            // Single Attr Ref
            if (currentIndex < delimited_query.size() - 2 &&
                delimited_query[currentIndex + 1] == ".")
            {
                tokens.push_back(PqlToken(TokenType::SYNONYM, delimited_query[currentIndex]));
                currentIndex += 1;

                tokens.push_back(PqlToken(TokenType::DOT, delimited_query[currentIndex]));
                currentIndex += 1;

                if (checkIfAttrName(delimited_query[currentIndex])) {
                    TokenType attrName = stringToTokenMap[delimited_query[currentIndex]];
                    tokens.push_back(PqlToken(attrName, delimited_query[currentIndex]));
                }
                else {
                    tokens.push_back(PqlToken(TokenType::UNKNOWN, delimited_query[currentIndex]));
                }
            }

            // Single Synonym
            else {
                tokens.push_back(PqlToken(TokenType::SYNONYM, delimited_query[currentIndex]));
            }
        }

        // If our single select is not BOOLEAN and does not start with a synonym, validator will throw error anyway
        else {
            tokens.push_back(PqlToken(TokenType::UNKNOWN, delimited_query[currentIndex]));
        }

        currentIndex += 1;
        return;
    }

    // Tuple of elems
    else {
        TokenType currentToken;
        ClauseType type = ClauseType::NONE;
        int firstElement = true;
        bool isCurrentStringSynonym = true;

        while (currentIndex < delimited_query.size()) {
            /*
            The only valid symbols are "<", ">", ".", ","
            If it is not a synonym, checkTokenType will assign whatever token the string is,
            and validator will catch this error
            */

            currentToken = checkValidSymbol(delimited_query[currentIndex])
                ? stringToTokenMap[delimited_query[currentIndex]]
                : checkSelectTokenType(delimited_query[currentIndex], isCurrentStringSynonym);


            if (currentToken == TokenType::DOT) {
                isCurrentStringSynonym = false;
            }

            if (checkIfAttrName(delimited_query[currentIndex])) {
                isCurrentStringSynonym = true;
            }
           
            tokens.push_back(PqlToken(currentToken, delimited_query[currentIndex]));
            currentIndex += 1;

            // For multi element Select return types
            if (currentToken == TokenType::CLOSED_ARROW) {
                return;
            }
        }
    }
}

TokenType SelectTokenizer::checkSelectTokenType(const string& s, const bool isCurrentSynonym) {
    if (!isCurrentSynonym && checkIfAttrName(s)) {
        return stringToTokenMap[s];
    }
    else {
        return checkIfSynonym(s) ? TokenType::SYNONYM : TokenType::UNKNOWN;
    }
}
