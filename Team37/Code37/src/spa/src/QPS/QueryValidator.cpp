using namespace std;

#include <string>
#include <set>
#include <vector>

#include "QPS.h"
#include "QueryValidator.h"

QueryValidator::QueryValidator(vector<PqlToken> &tokens) {
    next = tokens.begin();
    end = tokens.end();
    hasSelect = false;
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

    if (!errorFound())
    {
	    validateRequirements();
    }
    
    return pe;
}

void QueryValidator::validateDeclarations()
{
    PqlToken declarationType = getNextToken();
	while (declarationType.type != TokenType::END) // Change tokentype end to the correct type
	{
        if (!isValidDeclarationType(declarationType.type))
        {
            updatePqlError(ErrorType::SEMANTIC_ERROR, declarationType.value + " is not a valid declaration type");
            return;
        }

        PqlToken synonym = getNextToken();
        if (synonym.type != TokenType::SYNONYM)
        {
            updatePqlError(ErrorType::SEMANTIC_ERROR, "Declarations must be a synonym");
            return;
        }

        PqlToken semicolon = getNextToken();
        if (semicolon.type != TokenType::SEMICOLON)
        {
            updatePqlError(ErrorType::SYNTAX_ERROR, "Declarations must be followed with a semicolon");
            return;
        }

        declarations[synonym.value] = declarationType.type;
        declarationType = getNextToken();
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
    hasSelect = true;
    return;
}

void QueryValidator::validateClauses()
{
    PqlToken curr = getNextToken();

}

void QueryValidator::validateRequirements()
{
    if (!hasDeclarations)
    {
        updatePqlError(ErrorType::SEMANTIC_ERROR, "Query must have declarations");
        return;
    }

	if (!hasSelect)
	{
        updatePqlError(ErrorType::SEMANTIC_ERROR, "Query must have Select clause");
        return;
	}
    return;
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