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
	PqlToken(TokenType::ASSIGN, "statement"),
	PqlToken(TokenType::SYNONYM, "s"),
	PqlToken(TokenType::SEMICOLON, ";"),
	PqlToken(TokenType::DECLARATION_END, ""),
	PqlToken(TokenType::SELECT, "select"),
	PqlToken(TokenType::SYNONYM, "v")
};

// vector<PqlToken> valid_pattern = addPatternClause(basic_tokens);

//vector<PqlToken> valid_uses = addUsesClause(basic_tokens);

//vector<PqlToken> valid_modifies = addModifiesClause(basic_tokens);

//vector<PqlToken> valid_follows = addFollowsClause(basic_tokens);

//vector<PqlToken> valid_parent = addParentClause(basic_tokens);

vector<PqlToken> multi_declarations = {
	PqlToken(TokenType::VARIABLE, "variable"),
	PqlToken(TokenType::SYNONYM, "v1"),
	PqlToken(TokenType::COMMA, ","),
	PqlToken(TokenType::SYNONYM, "v2"),
	PqlToken(TokenType::COMMA, ","),
	PqlToken(TokenType::SYNONYM, "v3"),
	PqlToken(TokenType::SEMICOLON, ";"),
	PqlToken(TokenType::DECLARATION_END, ""),
	PqlToken(TokenType::SELECT, "select"),
	PqlToken(TokenType::SYNONYM, "v1")
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

vector<PqlToken> undeclared_pattern_assign = {
	PqlToken(TokenType::VARIABLE, "variable"),
	PqlToken(TokenType::SYNONYM, "v"),
	PqlToken(TokenType::SEMICOLON, ";"),
	PqlToken(TokenType::DECLARATION_END, ""),
	PqlToken(TokenType::SELECT, "select"),
	PqlToken(TokenType::SYNONYM, "v"),
	PqlToken(TokenType::PATTERN, "pattern"),
	PqlToken(TokenType::SYNONYM, "a"),
	PqlToken(TokenType::OPEN_BRACKET, "("),
	PqlToken(TokenType::SYNONYM, "v"),
	PqlToken(TokenType::COMMA, ","),
	PqlToken(TokenType::WILDCARD, "_"),
	PqlToken(TokenType::CLOSED_BRACKET, ")")
};

vector<PqlToken> undeclared_pattern_parameter = {
	PqlToken(TokenType::VARIABLE, "variable"),
	PqlToken(TokenType::SYNONYM, "v"),
	PqlToken(TokenType::SEMICOLON, ";"),
	PqlToken(TokenType::ASSIGN, "assign"),
	PqlToken(TokenType::SYNONYM, "a"),
	PqlToken(TokenType::SEMICOLON, ";"),
	PqlToken(TokenType::DECLARATION_END, ""),
	PqlToken(TokenType::SELECT, "select"),
	PqlToken(TokenType::SYNONYM, "v"),
	PqlToken(TokenType::PATTERN, "pattern"),
	PqlToken(TokenType::SYNONYM, "a"),
	PqlToken(TokenType::OPEN_BRACKET, "("),
	PqlToken(TokenType::SYNONYM, "v1"),
	PqlToken(TokenType::COMMA, ","),
	PqlToken(TokenType::WILDCARD, "_"),
	PqlToken(TokenType::CLOSED_BRACKET, ")")
};

vector<PqlToken> invalid_pattern_parameter = {
	PqlToken(TokenType::VARIABLE, "variable"),
	PqlToken(TokenType::SYNONYM, "v"),
	PqlToken(TokenType::SEMICOLON, ";"),
	PqlToken(TokenType::ASSIGN, "assign"),
	PqlToken(TokenType::SYNONYM, "a"),
	PqlToken(TokenType::SEMICOLON, ";"),
	PqlToken(TokenType::DECLARATION_END, ""),
	PqlToken(TokenType::SELECT, "select"),
	PqlToken(TokenType::SYNONYM, "v"),
	PqlToken(TokenType::PATTERN, "pattern"),
	PqlToken(TokenType::SYNONYM, "a"),
	PqlToken(TokenType::OPEN_BRACKET, "("),
	PqlToken(TokenType::STATEMENT_NUM, "1"),
	PqlToken(TokenType::COMMA, ","),
	PqlToken(TokenType::WILDCARD, "_"),
	PqlToken(TokenType::CLOSED_BRACKET, ")")
};

vector<PqlToken> pattern_missing_open_bracket = {
	PqlToken(TokenType::VARIABLE, "variable"),
	PqlToken(TokenType::SYNONYM, "v"),
	PqlToken(TokenType::SEMICOLON, ";"),
	PqlToken(TokenType::ASSIGN, "assign"),
	PqlToken(TokenType::SYNONYM, "a"),
	PqlToken(TokenType::SEMICOLON, ";"),
	PqlToken(TokenType::DECLARATION_END, ""),
	PqlToken(TokenType::SELECT, "select"),
	PqlToken(TokenType::SYNONYM, "v"),
	PqlToken(TokenType::PATTERN, "pattern"),
	PqlToken(TokenType::SYNONYM, "a"),
	PqlToken(TokenType::SYNONYM, "v"),
	PqlToken(TokenType::COMMA, ","),
	PqlToken(TokenType::WILDCARD, "_"),
	PqlToken(TokenType::CLOSED_BRACKET, ")")
};

vector<PqlToken> missing_such_that = {
	PqlToken(TokenType::VARIABLE, "variable"),
	PqlToken(TokenType::SYNONYM, "v"),
	PqlToken(TokenType::SEMICOLON, ";"),
	PqlToken(TokenType::DECLARATION_END, ""),
	PqlToken(TokenType::SELECT, "select"),
	PqlToken(TokenType::SYNONYM, "v"),
	PqlToken(TokenType::USES, "uses"),
	PqlToken(TokenType::OPEN_BRACKET, "("),
	PqlToken(TokenType::STATEMENT_NUM, "1"),
	PqlToken(TokenType::COMMA, ","),
	PqlToken(TokenType::SYNONYM, "v"),
	PqlToken(TokenType::CLOSED_BRACKET, ")")
};

vector<PqlToken> addPatternClause(vector<PqlToken> tokens)
{
	tokens.push_back(PqlToken(TokenType::PATTERN, "pattern"));
	tokens.push_back(PqlToken(TokenType::SYNONYM, "a"));
	tokens.push_back(PqlToken(TokenType::OPEN_BRACKET, "("));
	tokens.push_back(PqlToken(TokenType::SYNONYM, "v"));
	tokens.push_back(PqlToken(TokenType::COMMA, ","));
	tokens.push_back(PqlToken(TokenType::WILDCARD, "_"));
	tokens.push_back(PqlToken(TokenType::CLOSED_BRACKET, ")"));
	return tokens;
}

vector<PqlToken> addUsesClause(vector<PqlToken> tokens)
{
	tokens.push_back(PqlToken(TokenType::SUCH, "such"));
	tokens.push_back(PqlToken(TokenType::THAT, "that"));
	tokens.push_back(PqlToken(TokenType::USES, "uses"));
	tokens.push_back(PqlToken(TokenType::OPEN_BRACKET, "("));
	tokens.push_back(PqlToken(TokenType::STATEMENT_NUM, "1"));
	tokens.push_back(PqlToken(TokenType::COMMA, ","));
	tokens.push_back(PqlToken(TokenType::SYNONYM, "v"));
	tokens.push_back(PqlToken(TokenType::CLOSED_BRACKET, ")"));
	return tokens;
}

vector<PqlToken> addModifiesClause(vector<PqlToken> tokens)
{
	tokens.push_back(PqlToken(TokenType::SUCH, "such"));
	tokens.push_back(PqlToken(TokenType::THAT, "that"));
	tokens.push_back(PqlToken(TokenType::MODIFIES, "modifies"));
	tokens.push_back(PqlToken(TokenType::OPEN_BRACKET, "("));
	tokens.push_back(PqlToken(TokenType::STATEMENT_NUM, "1"));
	tokens.push_back(PqlToken(TokenType::COMMA, ","));
	tokens.push_back(PqlToken(TokenType::SYNONYM, "v"));
	tokens.push_back(PqlToken(TokenType::CLOSED_BRACKET, ")"));
	return tokens;
}

vector<PqlToken> addParentClause(vector<PqlToken> tokens)
{
	tokens.push_back(PqlToken(TokenType::SUCH, "such"));
	tokens.push_back(PqlToken(TokenType::THAT, "that"));
	tokens.push_back(PqlToken(TokenType::PARENT, "parent"));
	tokens.push_back(PqlToken(TokenType::OPEN_BRACKET, "("));
	tokens.push_back(PqlToken(TokenType::SYNONYM, "s"));
	tokens.push_back(PqlToken(TokenType::COMMA, ","));
	tokens.push_back(PqlToken(TokenType::STATEMENT_NUM, "1"));
	tokens.push_back(PqlToken(TokenType::CLOSED_BRACKET, ")"));
	return tokens;
}

vector<PqlToken> addFollowsClause(vector<PqlToken> tokens)
{
	tokens.push_back(PqlToken(TokenType::SUCH, "such"));
	tokens.push_back(PqlToken(TokenType::THAT, "that"));
	tokens.push_back(PqlToken(TokenType::FOLLOWS, "follows"));
	tokens.push_back(PqlToken(TokenType::OPEN_BRACKET, "("));
	tokens.push_back(PqlToken(TokenType::SYNONYM, "s"));
	tokens.push_back(PqlToken(TokenType::COMMA, ","));
	tokens.push_back(PqlToken(TokenType::STATEMENT_NUM, "1"));
	tokens.push_back(PqlToken(TokenType::CLOSED_BRACKET, ")"));
	return tokens;
}