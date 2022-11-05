using namespace std;

#include <unordered_map>

#include "../Structures/PqlError.h"
#include "../Structures/PqlToken.h"
#include "../Structures/PqlQuery.h"
#include "../Types/ErrorType.h"
#include "../Types/TokenType.h"
#include "CallsValidator.h"
#include "ClauseValidator.h"
#include "ValidatorUtils.h"
#include <iostream>

CallsValidator::CallsValidator(unordered_map<string, TokenType> *declarations, TokenType token) : ClauseValidator(declarations, token) {}

void CallsValidator::validate(PqlToken* left, PqlToken* right)
{
	validateEntityRef(left, TokenType::PROCEDURE);
	validateEntityRef(right, TokenType::PROCEDURE);
}
