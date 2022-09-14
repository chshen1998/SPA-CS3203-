#include "QPS/QueryExtractor.h"
#include "QPS/QPS.h"
#include "TestQueryExtractorUtils.h"

#include <vector>
#include <unordered_map>

#include "catch.hpp"
using namespace std;

vector<PqlToken> basic_tokens = {
	PqlToken(TokenType::VARIABLE, "variable"),
	PqlToken(TokenType::SYNONYM, "v"),
	PqlToken(TokenType::SEMICOLON, ";"),
	PqlToken(TokenType::ASSIGN, "assign"),
	PqlToken(TokenType::SYNONYM, "a"),
	PqlToken(TokenType::SEMICOLON, ";"),
	PqlToken(TokenType::DECLARATION_END, ""),
	PqlToken(TokenType::SELECT, "select"),
	PqlToken(TokenType::SYNONYM, "v")
};

vector<PqlToken> missing_semicolon = {
	PqlToken(TokenType::VARIABLE, "variable"),
	PqlToken(TokenType::SYNONYM, "v"),
	PqlToken(TokenType::DECLARATION_END, ""),
	PqlToken(TokenType::SELECT, "select"),
	PqlToken(TokenType::SYNONYM, "v")
};

vector<PqlToken> missing_select= {
	PqlToken(TokenType::VARIABLE, "variable"),
	PqlToken(TokenType::SYNONYM, "v"),
	PqlToken(TokenType::SEMICOLON, ";"),
	PqlToken(TokenType::DECLARATION_END, ""),
};

vector<PqlToken> undeclared_select_parameter = {
	PqlToken(TokenType::ASSIGN, "assign"),
	PqlToken(TokenType::SYNONYM, "a"),
	PqlToken(TokenType::SEMICOLON, ";"),
	PqlToken(TokenType::DECLARATION_END, ""),
	PqlToken(TokenType::SELECT, "select"),
	PqlToken(TokenType::SYNONYM, "v")
};

vector<PqlToken> addPatternClause(vector<PqlToken> tokens)
{
	tokens.push_back(PqlToken(TokenType::PATTERN, "pattern"));
	tokens.push_back(PqlToken(TokenType::SYNONYM, "a"));
	tokens.push_back(PqlToken(TokenType::OPEN_BRACKET, "("));
	tokens.push_back(PqlToken(TokenType::VARIABLE, "x"));
	tokens.push_back(PqlToken(TokenType::COMMA, ","));
	tokens.push_back(PqlToken(TokenType::VARIABLE, "y"));
	tokens.push_back(PqlToken(TokenType::CLOSED_BRACKET, ")"));
	return tokens;
}

vector<PqlToken> addUsesClause(vector<PqlToken> tokens)
{
	tokens.push_back(PqlToken(TokenType::SUCH, "such"));
	tokens.push_back(PqlToken(TokenType::THAT, "that"));
	tokens.push_back(PqlToken(TokenType::USES, "uses"));
	tokens.push_back(PqlToken(TokenType::OPEN_BRACKET, "("));
	tokens.push_back(PqlToken(TokenType::STATEMENT, "s"));
	tokens.push_back(PqlToken(TokenType::COMMA, ","));
	tokens.push_back(PqlToken(TokenType::VARIABLE, "v"));
	tokens.push_back(PqlToken(TokenType::CLOSED_BRACKET, ")"));
	return tokens;
}

vector<PqlToken> addModifiesClause(vector<PqlToken> tokens)
{
	tokens.push_back(PqlToken(TokenType::SUCH, "such"));
	tokens.push_back(PqlToken(TokenType::THAT, "that"));
	tokens.push_back(PqlToken(TokenType::USES, "uses"));
	tokens.push_back(PqlToken(TokenType::OPEN_BRACKET, "("));
	tokens.push_back(PqlToken(TokenType::STATEMENT, "s"));
	tokens.push_back(PqlToken(TokenType::COMMA, ","));
	tokens.push_back(PqlToken(TokenType::VARIABLE, "v"));
	tokens.push_back(PqlToken(TokenType::CLOSED_BRACKET, ")"));
	return tokens;
}

vector<PqlToken> addParentClause(vector<PqlToken> tokens)
{
	tokens.push_back(PqlToken(TokenType::SUCH, "such"));
	tokens.push_back(PqlToken(TokenType::THAT, "that"));
	tokens.push_back(PqlToken(TokenType::USES, "uses"));
	tokens.push_back(PqlToken(TokenType::OPEN_BRACKET, "("));
	tokens.push_back(PqlToken(TokenType::STATEMENT, "s1"));
	tokens.push_back(PqlToken(TokenType::COMMA, ","));
	tokens.push_back(PqlToken(TokenType::STATEMENT, "s2"));
	tokens.push_back(PqlToken(TokenType::CLOSED_BRACKET, ")"));
	return tokens;
}

vector<PqlToken> addFollowsClause(vector<PqlToken> tokens)
{
	tokens.push_back(PqlToken(TokenType::SUCH, "such"));
	tokens.push_back(PqlToken(TokenType::THAT, "that"));
	tokens.push_back(PqlToken(TokenType::USES, "uses"));
	tokens.push_back(PqlToken(TokenType::OPEN_BRACKET, "("));
	tokens.push_back(PqlToken(TokenType::STATEMENT, "s1"));
	tokens.push_back(PqlToken(TokenType::COMMA, ","));
	tokens.push_back(PqlToken(TokenType::STATEMENT, "s2"));
	tokens.push_back(PqlToken(TokenType::CLOSED_BRACKET, ")"));
	return tokens;
}