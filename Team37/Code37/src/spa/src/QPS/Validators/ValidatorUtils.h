#pragma once

#ifndef TEAM37_VALIDATORUTILS_H
#define TEAM37_VALIDATORUTILS_H

#include <string>
#include <set>
#include <vector>
#include <unordered_map>

#include "../QPS.h"

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

#endif //TEAM37_VALIDATORUTILS_H