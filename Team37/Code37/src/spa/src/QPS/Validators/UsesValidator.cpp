using namespace std;

#include <unordered_map>

#include "../Structures/PqlError.h"
#include "../Structures/PqlToken.h"
#include "../Structures/PqlQuery.h"
#include "../Types/ErrorType.h"
#include "../Types/TokenType.h"
#include "UsesValidator.h"
#include "ClauseValidator.h"
#include "ValidatorUtils.h"
#include <iostream>

set<TokenType> validUsesStmtParamTypes = {
	TokenType::ASSIGN,
	TokenType::PRINT,
	TokenType::STATEMENT,
	TokenType::IF,
	TokenType::WHILE,
	TokenType::CALL,
	TokenType::PROCEDURE
};

UsesValidator::UsesValidator(unordered_map<string, TokenType> declarations, TokenType token) : ClauseValidator(declarations, token) {}

void UsesValidator::validate(PqlToken left, PqlToken right)
{
	if (left.type == TokenType::WILDCARD) {
		throw SemanticError("Uses clause arg1 cannot be wildcard");
	}

	if (validStatementRef.find(left.type) != validStatementRef.end())
	{
		validateStatementRef(left, "uses", validUsesStmtParamTypes);
	}
	else
	{
		validateEntityRef(left, "uses", entityTypes);
	}
	validateEntityRef(right, "uses", entityTypes);
}
