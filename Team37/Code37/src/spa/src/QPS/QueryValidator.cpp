using namespace std;

#include <string>
#include <set>
#include <vector>

#include "QPS.h"
#include "QueryValidator.h"
#include "Validators/ClauseValidator.h"
#include "Validators/DeclarationValidator.h"
#include "Validators/FollowsValidator.h"
#include "Validators/ModifiesValidator.h"
#include "Validators/PatternValidator.h"
#include "Validators/ParentValidator.h"
#include "Validators/SelectValidator.h"
#include "Validators/UsesValidator.h"
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
    }

	if (curr.type == TokenType::PATTERN)
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
            throw SemanticError("Invalid characters after such that clause");
        }

        validatePattern();
    }

    curr = getNextToken();
    if (curr.type != TokenType::END)
    {
        throw SyntaxError("Invalid characters at end of query");
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


unique_ptr<ClauseValidator> QueryValidator::createClauseValidator(TokenType type)
{
    if (type == TokenType::USES) 
    {
        return make_unique<UsesValidator>(declarations);
    } else if (type == TokenType::FOLLOWS)
    {
        return make_unique<FollowsValidator>(declarations);
    } else if (type == TokenType::MODIFIES)
    {
        return make_unique<ModifiesValidator>(declarations);
    } else if (type == TokenType::PARENT)
    {
        return make_unique<ParentValidator>(declarations);
    } else
    {
        throw SemanticError("Invalid relationship type");
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
