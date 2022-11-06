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

QueryValidator::QueryValidator(vector<PqlToken> *tokenVector) {
    tokens = tokenVector;
    size = tokens->size();
    next = 0;
    booleanIsSynonym = false;
}

PqlError QueryValidator::validateQuery()
{
    try {
        validateDeclarations();
    
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

void QueryValidator::validateDeclarations()
{
    vector<PqlToken> declarationTokens;
    while (next < size && tokens->at(next).type != TokenType::DECLARATION_END) {
        declarationTokens.push_back(tokens->at(next));
        next++;
    }

    DeclarationValidator validator = DeclarationValidator(&declarationTokens, &declarations);
    validator.validate();

    if (declarations.find("BOOLEAN") != declarations.end()) {
        booleanIsSynonym = true;
    }
    next++;
}

void QueryValidator::validateSelect()
{
    SelectValidator validator = SelectValidator(&declarations);

    validator.validateSelect(getNextToken().type);
   
    PqlToken curr = getNextToken();
    if (curr.type == TokenType::OPEN_ARROW) {
        vector<PqlToken> tokens;

        curr = getNextToken(); // Get Token after open arrow
        while (curr.type != TokenType::CLOSED_ARROW) {
            tokens.push_back(curr);
            curr = getNextToken();
        }
        tokens.push_back(curr);
        validator.validateMultiple(&tokens);

        curr = getNextToken(); // Get Token after closed arros
    }
    else if (curr.type == TokenType::BOOLEAN) {
        // No validation required for boolean
        curr = getNextToken();
    } 
    else {
        validator.validateSynonym(curr);
        curr = getNextToken();

        // Check for select synonym attrName
        if (curr.type == TokenType::DOT) {
            PqlToken attrName = getNextToken();
            validator.validateAttrName(curr, attrName);

            curr = getNextToken(); // Get Token after attrName
        }
    }

    next--;
}

void QueryValidator::validateClauses()
{
    PqlToken curr = getNextToken();
    while (curr.type != TokenType::END)
    {
        switch (curr.type) {
        case TokenType::PATTERN:
            curr = validatePattern();
            break;
        case TokenType::WITH:
            curr = validateWith();
            break;
        case TokenType::SUCH:
            curr =validateSuchThat();
            break;
        default:
            throw SyntaxError("Invalid clauses");
        }
    }
}

PqlToken QueryValidator::validatePattern()
{
    PatternValidator validator = PatternValidator(&declarations);
    PqlToken andToken = PqlToken(TokenType::AND, "and");
    while (andToken.type == TokenType::AND)
    {
        PqlToken pattern = getNextToken();
        validator.validatePattern(&pattern);

        validator.validateOpen(getNextToken().type);
        PqlToken arg1 = getNextToken();
        validator.validateComma(getNextToken().type);
        PqlToken arg2 = getNextToken();

        switch (declarations[pattern.value]) {
        case TokenType::ASSIGN:
            validator.validateClose(getNextToken().type);
            validator.validate(&arg1, &arg2);
            break;
        case TokenType::WHILE:
            validator.validateClose(getNextToken().type);
            validator.validateWhile(&arg1, &arg2);
            break;
        case TokenType::IF:
            validator.validateComma(getNextToken().type);
            PqlToken arg3 = getNextToken();
            validator.validateClose(getNextToken().type);
            validator.validateIf(&arg1, &arg2, &arg3);
            break;
        }
        andToken = getNextToken();
    }

    return andToken;
}

PqlToken QueryValidator::validateWith()
{
    vector<PqlToken> withTokens;

    PqlToken nextToken = getNextToken();
    while (nextToken.type != TokenType::END && nextToken.type != TokenType::PATTERN && nextToken.type != TokenType::SUCH)
    {
        withTokens.push_back(nextToken);
        nextToken = getNextToken();
    }

    WithValidator validator = WithValidator(&declarations, &withTokens);
    validator.validate();

    return nextToken;
}

PqlToken QueryValidator::validateSuchThat()
{
    PqlToken that = getNextToken();
    if (that.type != TokenType::THAT)
    {
        throw SyntaxError("The keywords 'such that' must be used prior to a relationship reference");
    }

	PqlToken andToken = PqlToken(TokenType::AND, "and");
    while (andToken.type == TokenType::AND)
    {
        shared_ptr<ClauseValidator> validator = createClauseValidator(getNextToken().type);
        validator->validateOpen(getNextToken().type); 
        PqlToken left = getNextToken();
        validator->validateComma(getNextToken().type); 
        PqlToken right = getNextToken();
        validator->validateClose(getNextToken().type);
	    validator->validate(&left, &right);
        andToken = getNextToken();
    }

    return andToken;
}

 
shared_ptr<ClauseValidator> QueryValidator::createClauseValidator(TokenType type)
{
    if (type == TokenType::USES) 
    {
        return shared_ptr<UsesValidator>(new UsesValidator(&declarations, type));
    }  else if (type == TokenType::MODIFIES)
    {
        return shared_ptr<ModifiesValidator>(new ModifiesValidator(&declarations, type));
    }
    else if (type == TokenType::FOLLOWS || type == TokenType::FOLLOWS_A)
    {
        return shared_ptr<FollowsValidator>(new FollowsValidator(&declarations, type));
    }
    else if (type == TokenType::PARENT || type == TokenType::PARENT_A)
    {
        return shared_ptr<ParentValidator>(new ParentValidator(&declarations, type));
    }
    else if (type == TokenType::CALLS || type == TokenType::CALLS_A)
    {
        return shared_ptr<CallsValidator>(new CallsValidator(&declarations, type));
    }
    else if (type == TokenType::NEXT || type == TokenType::NEXT_A)
    {
        return shared_ptr<NextValidator>(new NextValidator(&declarations, type));
    }
    else if (type == TokenType::AFFECTS || type == TokenType::AFFECTS_A)
    {
        return shared_ptr<AffectsValidator>(new AffectsValidator(&declarations, type));
    }
    else
    {
        throw SemanticError("Invalid relationship type");
    }
}


PqlToken QueryValidator::getNextToken() {
    if (next >= size)
    {   
        next++;
        return PqlToken(TokenType::END, "");
    }
    PqlToken token = tokens->at(next);
    next++;
    if (token.type == TokenType::BOOLEAN && booleanIsSynonym) {
        token.type = TokenType::SYNONYM;
    }
    return token;

}
