using namespace std;

#include <unordered_map>
#include <vector>

#include "../Structures/PqlError.h"
#include "../Structures/PqlQuery.h"
#include "../Structures/PqlToken.h"
#include "../Types/ErrorType.h"
#include "../Types/TokenType.h"
#include "BaseValidator.h"
#include "ClauseValidator.h"
#include "SelectValidator.h"
#include "ValidatorUtils.h"

BaseValidator::BaseValidator()
{
}

PqlToken BaseValidator::getNextToken()
{
    if (next == size) {
        return PqlToken(TokenType::END, "");
    }
    PqlToken token = tokens->at(next);
    next = next + 1;
    return token;
}

bool BaseValidator::isDeclared(PqlToken* synonym)
{
    auto findit = declarations->find(synonym->value);
    return (findit != declarations->end());
}
