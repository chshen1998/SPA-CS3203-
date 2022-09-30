#pragma once

#ifndef TEAM37_VALIDATORUTILS_H
#define TEAM37_VALIDATORUTILS_H

#include <string>
#include <set>
#include <vector>
#include <unordered_map>

#include "../Structures/PqlError.h"
#include "../Structures/PqlToken.h"
#include "../Structures/PqlQuery.h"
#include "../Types/ErrorType.h"
#include "../Types/TokenType.h"

class SyntaxError : public exception {
public:
	SyntaxError(string message);
	string message;
};

class SemanticError : public exception {
public:
	SemanticError(string message);
	string message;
};


extern set<TokenType> validDesignEntities;

extern set<TokenType> validRelationshipsRef;

extern set<TokenType> validStatementRef;

extern set<TokenType> validEntityRef;

extern set<TokenType> validExpressionSpec;

extern set<TokenType> validAttrName;

extern set<TokenType> validPatternType;

extern unordered_map<TokenType, string> relationshipToStringMap;

#endif //TEAM37_VALIDATORUTILS_H