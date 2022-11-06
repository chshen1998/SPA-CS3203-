using namespace std;

#include <string>
#include <set>
#include <vector>
#include <unordered_map>

#include "../Structures/PqlError.h"
#include "../Structures/PqlToken.h"
#include "../Structures/PqlQuery.h"
#include "../Types/ErrorType.h"
#include "../Types/TokenType.h"
#include "DeclarationValidator.h"
#include "ValidatorUtils.h"
#include "BaseValidator.h"

DeclarationValidator::DeclarationValidator(vector<PqlToken>* declarationTokens, unordered_map<string, TokenType>* declarationMap) : BaseValidator{}
{
    tokens = declarationTokens;
    next = 0;
    size = tokens->size();
    declarations = declarationMap;
}

void DeclarationValidator::validate() {
    PqlToken declarationType = getNextToken();
    while (declarationType.type != TokenType::END)
    {
        isValidDesignEntity(&declarationType);
        PqlToken synonym = getNextToken();
        isValidSynonym(&synonym);
        PqlToken sign = getNextToken();
        isSemicolonOrComma(&sign);

        while (sign.type == TokenType::COMMA) {
            (*declarations)[synonym.value] = declarationType.type;
            synonym = getNextToken();
            isValidSynonym(&synonym);
            sign = getNextToken();
            isSemicolonOrComma(&sign);
        }

        (*declarations)[synonym.value] = declarationType.type;
        declarationType = getNextToken();
    }
}

void DeclarationValidator::isValidDesignEntity(PqlToken *token)
{
    if (!(validDesignEntities.find(token->type) != validDesignEntities.end())) {
        throw SyntaxError(token->value + " is not a valid design entity type");
    }
}

void DeclarationValidator::isValidSynonym(PqlToken *token) {
    if (token->type != TokenType::SYNONYM) {
        throw SyntaxError("Declarations must be a synonym");
    }
    else if (declarations->find(token->value) != declarations->end()) {
        throw SemanticError("Synonym name can only be declared once");
    }
}

void DeclarationValidator::isSemicolonOrComma(PqlToken *token) {
    if (token->type != TokenType::SEMICOLON && token->type != TokenType::COMMA) {
        throw SyntaxError("Declarations synonym must be followed with either a semicolon or comma");
    }
}