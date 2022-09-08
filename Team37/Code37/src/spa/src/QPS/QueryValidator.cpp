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
    PqlToken selectClause = validateDeclarations();
    if (errorFound())
    {
        return pe;
    }

    validateSelectClause(selectClause);
}

PqlToken QueryValidator::validateDeclarations()
{
    PqlToken declarationType = getNextToken();
	while (declarationType.type != TokenType::END)
	{
        if (!isValidDeclarationType(declarationType.type))
        {
            pe.type = ErrorType::SEMANTIC_ERROR;
            pe.message = declarationType.value + " is not a valid declaration type";
            return declarationType;
        }

        PqlToken synonym = getNextToken();

        PqlToken semicolon = getNextToken();
        if (semicolon.type != TokenType::SEMICOLON)
        {
            pe.type = ErrorType::SYNTAX_ERROR;
            pe.message = "Every declartion must end with a semicolon";
            return declarationType;
        }

        declarationType = getNextToken();
        hasDeclarations = true;
	}
    return declarationType;
}

void QueryValidator::validateSelectClause(PqlToken select)
{
	if (select.type != TokenType::SELECT)
	{
        pe.type = ErrorType::SEMANTIC_ERROR;
        pe.message = "Select clause must come after declarations";
        return;
	}

    PqlToken synonym = getNextToken();

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