using namespace std;

#include <unordered_map>
#include <vector>
#include <string>

#include "../Structures/PqlError.h"
#include "../Structures/PqlToken.h"
#include "../Structures/PqlQuery.h"
#include "../Types/ErrorType.h"
#include "../Types/TokenType.h"
#include "WithValidator.h"
#include "ClauseValidator.h"
#include "ValidatorUtils.h"

WithValidator::WithValidator(unordered_map<string, TokenType> declarationsMap, vector<PqlToken> withTokens)
{
	declarations = declarationsMap;
	tokens = withTokens;
	next = 0;
	size = tokens.size();
}

void WithValidator::validate()
{
    vector<PqlToken> refTokens;

    PqlToken curr = getNextToken();
    while (curr.type != TokenType::END)
    {
        refTokens.clear();
        while (curr.type != TokenType::EQUAL && curr.type != TokenType::END)
        {
            refTokens.push_back(curr);
            curr = getNextToken();
        }
        validateRef(refTokens);

        refTokens.clear();
        curr = getNextToken(); // Get token after "="
        while (curr.type != TokenType::AND && curr.type != TokenType::END)
        {
            refTokens.push_back(curr);
            curr = getNextToken();
        }
        validateRef(refTokens);

        curr = getNextToken(); // Get token after "and"
    }
}

void WithValidator::validateRef(vector<PqlToken> refTokens)
{
    if (refTokens.size() == 1)
    {
        TokenType type = refTokens[0].type;
	    if (type != TokenType::STRING && type != TokenType::NUMBER)
	    {
            throw SemanticError("Invalid With clause parameters 1");
	    }
    }
    else if (refTokens.size() == 3)
    {
        PqlToken synonym = refTokens[0];
        if (synonym.type != TokenType::SYNONYM)
        {
            throw SemanticError("Invalid With clause parameters 2");
        }
        if (declarations.find(synonym.value) == declarations.end())
        {
            throw SemanticError("Undeclared parameter in With clause");
        }

        PqlToken dot = refTokens[1];
        if (dot.type != TokenType::DOT)
        {
            throw SemanticError("Invalid With clause parameters 3");
        }
        
        PqlToken attrName = refTokens[2];
        if (validAttrName.find(attrName.type) == validAttrName.end())
        {
            throw SemanticError("Invalid With clause parameters 4");
        }
    } else
    {
        throw SemanticError("Invalid With Clause");
    }
}

PqlToken WithValidator::getNextToken() {
    if (next == size)
    {
        return PqlToken(TokenType::END, "");
    }
    PqlToken token = tokens[next];
    next = next + 1;
    return token;

}
