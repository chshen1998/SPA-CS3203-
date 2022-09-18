using namespace std;

#include <string>
#include <set>
#include <vector>
#include <unordered_map>

#include "../QPS.h"
#include "DeclarationValidator.h"
#include "ValidatorUtils.h"

DeclarationValidator::DeclarationValidator(vector<PqlToken> declarationTokens) {
    tokens = declarationTokens;
    next = 0;
    size = tokens.size();
}

unordered_map<string, TokenType> DeclarationValidator::validate() {
    unordered_map<string, TokenType> declarations;

    PqlToken declarationType = getNextToken();
    while (declarationType.type != TokenType::END)
    {
        isValidDesignEntity(declarationType);
        PqlToken synonym = getNextToken();
        isSynonym(synonym);
        PqlToken sign = getNextToken();
        isSemicolonOrComma(sign);

        while (sign.type == TokenType::COMMA) {
            declarations[synonym.value] = declarationType.type;
            synonym = getNextToken();
            isSynonym(synonym);
            sign = getNextToken();
            isSemicolonOrComma(sign);
        }

        declarations[synonym.value] = declarationType.type;
        declarationType = getNextToken();
    }

    return declarations;
}

void DeclarationValidator::isValidDesignEntity(PqlToken token)
{
    if (!(validDesignEntities.find(token.type) != validDesignEntities.end())) {
        throw SyntaxError(token.value + " is not a valid design entity type");
    }
}

void DeclarationValidator::isSynonym(PqlToken token) {
    if (token.type != TokenType::SYNONYM) {
        throw SyntaxError("Declarations must be a synonym");
    }
}

void DeclarationValidator::isSemicolonOrComma(PqlToken token) {
    if (token.type != TokenType::SEMICOLON && token.type != TokenType::COMMA) {
        throw SyntaxError("Declarations synonym must be followed with either a semicolon or comma");
    }
}

PqlToken DeclarationValidator::getNextToken() {
    if (next == size)
    {
        return PqlToken(TokenType::END, "");
    }
    PqlToken token = tokens[next];
    next = next + 1;
    return token;

}