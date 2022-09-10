using namespace std;

#include <string>
#include <set>
#include <vector>

#include "QPS.h"
#include "QueryValidator.h"
#include "Validators/ClauseValidator.h"
#include "Validators/DeclarationValidator.h"
#include "Validators/PatternValidator.h"
#include "Validators/SelectValidator.h"

QueryValidator::QueryValidator(vector<PqlToken> &tokens) {
    next = tokens.begin();
    end = tokens.end();
    pe.type = ErrorType::NONE;
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
    DeclarationValidator validator = DeclarationValidator();

    PqlToken declarationType = getNextToken();
    while (declarationType.type != TokenType::DECLARATION_END)
    {
        PqlToken synonym = getNextToken();
        PqlToken semicolon = getNextToken();

        pe = validator.validate(declarationType, synonym, semicolon);
        if (errorFound()) {
            return;
        }

        declarations[synonym.value] = declarationType.type;
        declarationType = getNextToken();
    }
}

void QueryValidator::validateSelect()
{
    SelectValidator validator = SelectValidator(declarations);
    PqlToken select = getNextToken();
    PqlToken synonym = getNextToken();

    pe = validator.validateParameters(select, synonym);
}

void QueryValidator::validateClauses()
{
    PqlToken curr = getNextToken();
    if (curr.type == TokenType::END)
    {
        return;
    } else if (curr.type == TokenType::PATTERN)
    {
        validatePattern();

        curr = getNextToken();
        if (curr.type == TokenType::END) { return; }

        validateSuchThat(curr);
    } else 
    {
        validateSuchThat(curr);

        curr = getNextToken();
        if (curr.type == TokenType::END)
        {
	        return;
        }
        else if (curr.type != TokenType::PATTERN)
        {
            pe.type = ErrorType::SEMANTIC_ERROR;
            pe.message = "Invalid characters after such that clause";
            return;
        }

        validatePattern();
    }

    curr = getNextToken();
    if (curr.type != TokenType::END)
    {
        pe.type = ErrorType::SYNTAX_ERROR;
        pe.message = "Invalid characters at end of query";
    }
}

void QueryValidator::validatePattern()
{
    PatternValidator validator = PatternValidator(declarations);
    pe = validator.validateAssign(getNextToken());
    if (errorFound()) { return; }

    PqlToken open = getNextToken();
    PqlToken left = getNextToken();
    PqlToken comma = getNextToken();
    PqlToken right = getNextToken();
    PqlToken close = getNextToken();
    pe = validator.validateBrackets(open, comma, close);
    pe = validator.validateParameters(left, right);
}

void QueryValidator::validateSuchThat(PqlToken such)
{
    PqlToken that = getNextToken();
    unique_ptr<ClauseValidator> validator = createClauseValidator(getNextToken().type);

    pe = validator->validateSuchThat(such, that);
    if (errorFound()) { return; }

    PqlToken open = getNextToken();
    PqlToken left = getNextToken();
    PqlToken comma = getNextToken();
    PqlToken right = getNextToken();
    PqlToken close = getNextToken();
    pe = validator->validateBrackets(open, comma, close);
    pe = validator->validateParameters(left, right);
}


bool QueryValidator::errorFound()
{
    return pe.type != ErrorType::NONE;
}

// TODO Implement Validators for such that clauses and complete this function
unique_ptr<ClauseValidator> QueryValidator::createClauseValidator(TokenType type)
{
    if (type == TokenType::USES) {
        return make_unique<PatternValidator>(declarations);
    }
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