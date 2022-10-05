using namespace std;

#include <string>

#include "../Structures/PqlToken.h"
#include "../Types/TokenType.h"
#include "DeclarationTokenizer.h"
#include "TokenizerUtils.h"
#include <iostream>

using namespace TokenizerUtils;

void DeclarationTokenizer::tokenize() {
    bool isCurrentStringSynonym = false; // If not, it must be a keyword TokenType such as a design entity / select etc.

    while (currentIndex < delimited_query.size()) {
        TokenType currentToken = TokenType::SYNONYM;

        if (delimited_query[currentIndex] == "Select" && 
           (currentIndex == 0 || tokens.back().type == TokenType::SEMICOLON)) {
            tokens.push_back(PqlToken(TokenType::DECLARATION_END, ""));
            return;
        }

        if (!isCurrentStringSynonym && (checkIfDesignEntity(delimited_query[currentIndex]))) {
            currentToken = stringToTokenMap[delimited_query[currentIndex]];
            isCurrentStringSynonym = true;
        }

        else if (delimited_query[currentIndex] == ";") {
            currentToken = TokenType::SEMICOLON;
            isCurrentStringSynonym = false;
        }

        else if (delimited_query[currentIndex] == ",") {
            currentToken = TokenType::COMMA;
        }

        else {
            // Similarly, this should be a synonym after a design entity, but we pass it whatever fits
            currentToken = checkIfSynonym(delimited_query[currentIndex])
                ? TokenType::SYNONYM
                : TokenType::UNKNOWN;
        }

        tokens.push_back(PqlToken(currentToken, delimited_query[currentIndex]));
        currentIndex += 1;
    }
}