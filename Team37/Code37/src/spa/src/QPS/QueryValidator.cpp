using namespace std;

#include <string>
#include <set>
#include <vector>

#include "QPS.h"
#include "QueryValidator.h"

QueryValidator::QueryValidator(vector<PqlToken> &tokens) {
    next = tokens.begin();
    end = tokens.end();
    hasSelectClause = false;
    hasDeclarations = false;
    pe = PqlError(ErrorType::NONE);
}

PqlError QueryValidator::ValidateQuery()
{
    validateDeclarations();

    if (!errorFound()) 
    {
        validateSelect();
    }

    if (!errorFound())
    {
	    validateClauses();
    }

    return pe;
}

void QueryValidator::validateDeclarations()
{
    PqlToken curr = getNextToken();
	while (curr.type != TokenType::END) // Change tokentype end to the correct type
	{
        if (!isValidDeclarationType(curr.type))
        {
            updatePqlError(ErrorType::SEMANTIC_ERROR, curr.value + " is not a valid declaration type");
            return;
        }

        curr = getNextToken();
        if (curr.type != TokenType::SYNONYM)
        {
            updatePqlError(ErrorType::SEMANTIC_ERROR, "Declarations must be a synonym");
            return;
        }

        curr = getNextToken();
        if (curr.type != TokenType::SEMICOLON)
        {
            updatePqlError(ErrorType::SYNTAX_ERROR, "Declarations must be followed with a semicolon");
            return;
        }

        curr = getNextToken();
        hasDeclarations = true;
	}
    return;
}

void QueryValidator::validateSelect()
{
    PqlToken curr = getNextToken();
	if (curr.type != TokenType::SELECT)
	{
        updatePqlError(ErrorType::SEMANTIC_ERROR, "Select must come after declarations");
        return;
	}

    curr = getNextToken();
    if (curr.type != TokenType::SYNONYM)
    {
        updatePqlError(ErrorType::SEMANTIC_ERROR, "Select must be followed with a declared synonym");
        return;
    }
    return;
}

void QueryValidator::validateClauses()
{
	
}

void QueryValidator::updatePqlError(ErrorType type, string msg)
{
    pe.type = type;
    pe.message = msg;
}

bool QueryValidator::isValidDeclarationType(TokenType type)
{
    if ((validDeclarations.find(type) != validDeclarations.end())) {
        return true;
    }
    return false;
}

bool QueryValidator::errorFound()
{
    return pe.type != ErrorType::NONE;
}


PqlToken QueryValidator::getNextToken()
{
    if (next == end) {
        return PqlToken(TokenType::END, "");
    }
    else {
        const PqlToken token = *next;
        next++;
        return token;
    }
}