using namespace std;

#include <string>
#include <set>
#include <vector>

#include "./Structures/PqlError.h"
#include "./Structures/PqlToken.h"
#include "./Structures/PqlQuery.h"
#include "./Types/ErrorType.h"
#include "./Types/TokenType.h"
#include "QueryValidator.h"
#include "Validators/AffectsValidator.h"
#include "Validators/CallsValidator.h"
#include "Validators/ClauseValidator.h"
#include "Validators/DeclarationValidator.h"
#include "Validators/FollowsValidator.h"
#include "Validators/ModifiesValidator.h"
#include "Validators/NextValidator.h"
#include "Validators/PatternValidator.h"
#include "Validators/ParentValidator.h"
#include "Validators/SelectValidator.h"
#include "Validators/UsesValidator.h"
#include "Validators/ValidatorUtils.h"
#include "Validators/WithValidator.h"
#include <iostream>

QueryValidator::QueryValidator(vector<PqlToken> tokenVector) {
    tokens = tokenVector;
    size = tokens.size();
    next = 0;
    booleanIsSynonym = false;
}

PqlError QueryValidator::validateQuery()
{
    try {
        declarations = validateDeclarations();

        if (declarations.find("BOOLEAN") != declarations.end()) {
            booleanIsSynonym = true;
        }
    
        PqlToken curr = validateSelect();

	    validateClauses(curr);

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

PqlToken QueryValidator::validateSelect()
{
    SelectValidator validator = SelectValidator(declarations);

    validator.validateSelect(getNextToken()); // Select token

   
    PqlToken curr = getNextToken();
    if (curr.type == TokenType::OPEN_ARROW) {
        vector<PqlToken> tokens;

        curr = getNextToken(); // Get Token after open arrow
        while (curr.type != TokenType::CLOSED_ARROW) {
            tokens.push_back(curr);
            curr = getNextToken();
        }
        tokens.push_back(curr);
        validator.validateMultiple(tokens);

        curr = getNextToken(); // Get Token after closed arros
    }
    else if (curr.type == TokenType::BOOLEAN) {
        // No validation required for boolean
        curr = getNextToken();
    } 
    else {
        // If not boolean then validate synonym
        validator.validateSynonym(curr);
        curr = getNextToken();

        // Check for select synonym attrName
        if (curr.type == TokenType::DOT) {
            PqlToken attrName = getNextToken();
            validator.validateAttrName(curr, attrName);

            curr = getNextToken(); // Get Token after attrName
        }
    }

    return curr;
}

void QueryValidator::validateClauses(PqlToken curr)
{
    while (curr.type != TokenType::END)
    {
	    if (curr.type == TokenType::PATTERN)
	    {
            curr = validatePattern();
	    }
    	else if (curr.type == TokenType::WITH)
	    {
            curr = validateWith();
	    }
        else if (curr.type == TokenType::SUCH)
        {
            curr = validateSuchThat(curr);
        }
        else
        {
            throw SyntaxError("Invalid clauses");
        }
    }
}

PqlToken QueryValidator::validatePattern()
{
    PatternValidator validator = PatternValidator(declarations, TokenType::PATTERN);

    PqlToken andToken = PqlToken(TokenType::AND, "and");
    while (andToken.type == TokenType::AND)
    {
        PqlToken pattern = getNextToken();
        validator.validatePattern(pattern);

        if (declarations[pattern.value] == TokenType::ASSIGN) {
            validator.validateOpen(getNextToken()); // Open bracket
	        PqlToken left = getNextToken();
            validator.validateComma(getNextToken()); // Comma
	        PqlToken right = getNextToken();
            validator.validateClose(getNextToken()); // Closed bracket
            validator.validate(left, right);
        }
        else if (declarations[pattern.value] == TokenType::WHILE) {
            validator.validateOpen(getNextToken()); // Open bracket
            PqlToken left = getNextToken();
            validator.validateComma(getNextToken()); // Comma
            PqlToken right = getNextToken();
            validator.validateClose(getNextToken()); // Closed bracket
            validator.validateWhile(left, right);
        }
        else { // IF
            validator.validateOpen(getNextToken()); // Open bracket
            PqlToken left = getNextToken();
            validator.validateComma(getNextToken()); // Comma
            PqlToken mid = getNextToken();
            validator.validateComma(getNextToken()); // Comma
            PqlToken right = getNextToken();
            validator.validateClose(getNextToken()); // Closed bracket
            validator.validateIf(left, mid, right);
        }
        andToken = getNextToken();
    }
    return andToken;
}

PqlToken QueryValidator::validateWith()
{
    vector<PqlToken> withTokens;

    PqlToken next = getNextToken();
    while (next.type != TokenType::END && next.type != TokenType::PATTERN && next.type != TokenType::SUCH)
    {
        withTokens.push_back(next);
        next = getNextToken();
    }

    WithValidator validator = WithValidator(declarations, withTokens);
    validator.validate();

    return next;
}

PqlToken QueryValidator::validateSuchThat(PqlToken such)
{
    PqlToken that = getNextToken();
    if (such.type != TokenType::SUCH || that.type != TokenType::THAT)
    {
        throw SyntaxError("The keywords 'such that' must be used prior to a relationship reference");
    }

	PqlToken andToken = PqlToken(TokenType::AND, "and");
    while (andToken.type == TokenType::AND)
    {
        shared_ptr<ClauseValidator> validator = createClauseValidator(getNextToken().type);
        validator->validateOpen(getNextToken()); // Open bracket
        PqlToken left = getNextToken();
        validator->validateComma(getNextToken()); // Comma
        PqlToken right = getNextToken();
        validator->validateClose(getNextToken()); // Closed bracket
	    validator->validate(left, right);
        andToken = getNextToken();
    }
    return andToken;
}

 
shared_ptr<ClauseValidator> QueryValidator::createClauseValidator(TokenType type)
{
    if (type == TokenType::USES) 
    {
        return shared_ptr<UsesValidator>(new UsesValidator(declarations, type));
    }  else if (type == TokenType::MODIFIES)
    {
        return shared_ptr<ModifiesValidator>(new ModifiesValidator(declarations, type));
    }
    else if (type == TokenType::FOLLOWS || type == TokenType::FOLLOWS_A)
    {
        return shared_ptr<FollowsValidator>(new FollowsValidator(declarations, type));
    }
    else if (type == TokenType::PARENT || type == TokenType::PARENT_A)
    {
        return shared_ptr<ParentValidator>(new ParentValidator(declarations, type));
    }
    else if (type == TokenType::CALLS || type == TokenType::CALLS_A)
    {
        return shared_ptr<CallsValidator>(new CallsValidator(declarations, type));
    }
    else if (type == TokenType::NEXT || type == TokenType::NEXT_A)
    {
        return shared_ptr<NextValidator>(new NextValidator(declarations, type));
    }
    else if (type == TokenType::AFFECTS || type == TokenType::AFFECTS_A)
    {
        return shared_ptr<AffectsValidator>(new AffectsValidator(declarations, type));
    }
    else
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
    if (token.type == TokenType::BOOLEAN && booleanIsSynonym) {
        token.type = TokenType::SYNONYM;
    }
    return token;

}
