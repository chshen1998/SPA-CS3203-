using namespace std;

#include <unordered_map>

#include "../Structures/PqlError.h"
#include "../Structures/PqlQuery.h"
#include "../Structures/PqlToken.h"
#include "../Types/ErrorType.h"
#include "../Types/TokenType.h"
#include "ClauseValidator.h"
#include "UsesValidator.h"
#include "ValidatorUtils.h"
#include <iostream>

UsesValidator::UsesValidator(unordered_map<string, TokenType>* declarations, TokenType token)
    : ClauseValidator(declarations, token)
{
}

void UsesValidator::validate(PqlToken* left, PqlToken* right)
{
    if (left->type == TokenType::WILDCARD) {
        throw SemanticError("Uses clause arg1 cannot be wildcard");
    }

    if (validStatementRef.find(left->type) != validStatementRef.end()) {
        validateStatementRef(left);
    } else {
        validateEntityRef(left, TokenType::VARIABLE);
    }
    validateEntityRef(right, TokenType::VARIABLE);
}
