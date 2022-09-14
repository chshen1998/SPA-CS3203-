#pragma once

#include <string>
#include <set>
#include <vector>
#include <unordered_map>

#include "../QPS.h"

#ifndef TEAM37_VALIDATORUTILS_H
#define TEAM37_VALIDATORUTILS_H

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

extern set<TokenType> validPatternParameterTypes;

extern set<TokenType> validFollowsParameterTypes;

extern set<TokenType> validUsesParameterTypes;

extern set<TokenType> validModifiesParameterTypes;

extern set<TokenType> validParentParameterTypes;


#endif //TEAM37_VALIDATORUTILS_H