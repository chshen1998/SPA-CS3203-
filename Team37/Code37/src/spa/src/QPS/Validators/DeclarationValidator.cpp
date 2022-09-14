using namespace std;

#include <string>
#include <set>
#include <vector>
#include <unordered_map>

#include "../QPS.h"
#include "DeclarationValidator.h"
#include "Validator.h"

DeclarationValidator::DeclarationValidator() : Validator() {}

PqlError DeclarationValidator::validate(PqlToken declarationType, PqlToken synonym, PqlToken semicolon) {
    if (!isValidDeclarationType(declarationType.type))
    {
        updatePqlError(ErrorType::SYNTAX_ERROR, declarationType.value + " is not a valid declaration type");
    } else if (synonym.type != TokenType::SYNONYM)
    {
        updatePqlError(ErrorType::SEMANTIC_ERROR, "Declarations must be a synonym");
    } else if (semicolon.type != TokenType::SEMICOLON)
    {
        updatePqlError(ErrorType::SYNTAX_ERROR, declarationType.value + "Declarations must be followed with a semicolon");
    }
    
    return pe;
}

bool DeclarationValidator::isValidDeclarationType(TokenType type)
{
    if ((validDeclarations.find(type) != validDeclarations.end())) {
        return true;
    }
    return false;
}