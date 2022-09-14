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
#include "Validators/ValidatorUtils.h"

QueryValidator::QueryValidator(vector<PqlToken> tokenVector) {
    tokens = tokenVector;
    size = tokens.size();
    next = 0;
}

PqlError QueryValidator::ValidateQuery()
{
    try {
        declarations = validateDeclarations();
    
        validateSelect();

	    validateClauses();

        return PqlError(ErrorType::NONE, "");
    }
    catch (SyntaxError e) {
        return PqlError(ErrorType::SYNTAX_ERROR, e.message);
    } 
    catch (SemanticError e) {
        return PqlError(ErrorType::SEMANTIC_ERROR, e.message);
    }
}

unordered_map<string, TokenType> QueryValidator::validateDeclarations()
{
    vector<PqlToken> declarationTokens;
    PqlToken curr = getNextToken();
    while (curr.type != TokenType::DECLARATION_END && curr.type != TokenType::END) {
        declarationTokens.push_back(curr);
        curr = getNextToken();
    }

    DeclarationValidator validator = DeclarationValidator(declarationTokens);
    unordered_map<string, TokenType> declarations = validator.validate();
    return declarations;
}

void QueryValidator::validateSelect()
{
    SelectValidator validator = SelectValidator(declarations);
    PqlToken select = getNextToken();
    PqlToken synonym = getNextToken();

    validator.validate(select, synonym);
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
           // pe.errorType = ErrorType::SEMANTIC_ERROR;
           // pe.message = "Invalid characters after such that clause";
            return;
        }

        validatePattern();
    }

    curr = getNextToken();
    if (curr.type != TokenType::END)
    {
        //pe.errorType = ErrorType::SYNTAX_ERROR;
        //pe.message = "Invalid characters at end of query";
    }
}

void QueryValidator::validatePattern()
{
    PatternValidator validator = PatternValidator(declarations);
    validator.validateAssign(getNextToken());

    PqlToken open = getNextToken();
    PqlToken left = getNextToken();
    PqlToken comma = getNextToken();
    PqlToken right = getNextToken();
    PqlToken close = getNextToken();
    validator.validateBrackets(open, comma, close);
    validator.validate(left, right);
}

void QueryValidator::validateSuchThat(PqlToken such)
{
    PqlToken that = getNextToken();
    unique_ptr<ClauseValidator> validator = createClauseValidator(getNextToken().type);

    validator->validateSuchThat(such, that);

    PqlToken open = getNextToken();
    PqlToken left = getNextToken();
    PqlToken comma = getNextToken();
    PqlToken right = getNextToken();
    PqlToken close = getNextToken();
    validator->validateBrackets(open, comma, close);
    validator->validate(left, right);
}


// TODO Implement Validators for such that clauses and complete this function
unique_ptr<ClauseValidator> QueryValidator::createClauseValidator(TokenType type)
{
    if (type == TokenType::USES) {
        return make_unique<PatternValidator>(declarations);
    }
}


PqlToken QueryValidator::getNextToken() {
    if (next == size)
    {
        return PqlToken(TokenType::END, "");
    }
    PqlToken token = tokens[next];
    next = next + 1;
    return token;

}
