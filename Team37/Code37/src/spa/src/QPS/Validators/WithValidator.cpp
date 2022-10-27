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

WithValidator::WithValidator(unordered_map<string, TokenType> *declarationsMap, vector<PqlToken> *withTokens)
{
	declarations = declarationsMap;
	tokens = withTokens;
	next = 0;
	size = tokens->size();
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
        TokenType leftType = validateRef(refTokens);

        refTokens.clear();
        curr = getNextToken(); // Get token after "="
        while (curr.type != TokenType::AND && curr.type != TokenType::END)
        {
            refTokens.push_back(curr);
            curr = getNextToken();
        }
        TokenType rightType = validateRef(refTokens);

        if (leftType != rightType) {
            throw SemanticError("For attrCompare, the two ref comparison must be of the same type");
        }

        curr = getNextToken(); // Get token after "and"
    }
}

TokenType WithValidator::validateRef(vector<PqlToken> refTokens)
{
    if (refTokens.size() == 1)
    {
        TokenType type = refTokens[0].type;
	    if (type != TokenType::STRING && type != TokenType::NUMBER)
	    {
            throw SemanticError("Invalid With clause parameters 1");
	    }
        return type;
    }
    else if (refTokens.size() == 3)
    {
        PqlToken synonym = refTokens[0];
        if (synonym.type != TokenType::SYNONYM)
        {
            throw SemanticError("Invalid With clause parameters 2");
        }
        if (declarations->find(synonym.value) == declarations->end())
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

        TokenType t = declarations->at(synonym.value);
        if (validSynonymToAttrMap[t].find(attrName.type) == validSynonymToAttrMap[t].end()) {
            throw SemanticError("Invalid attrName for attrRef synonym");
        }


        if (attrName.type == TokenType::PROCNAME || attrName.type == TokenType::VARNAME) {
            return TokenType::STRING;
        }
        else {
            return TokenType::NUMBER;
        }

    } else
    {
        throw SemanticError("Invalid With Clause");
        return TokenType::NONE;
    }
}

PqlToken WithValidator::getNextToken() {
    if (next == size)
    {
        return PqlToken(TokenType::END, "");
    }
    PqlToken token = tokens->at(next);
    next = next + 1;
    return token;

}
