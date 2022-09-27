#include "QPS/QueryExtractor.h"
#include "QPS/Structures/PqlError.h"
#include "QPS/Structures/PqlToken.h"
#include "QPS/Structures/PqlQuery.h"
#include "QPS/Types/ErrorType.h"
#include "QPS/Types/TokenType.h"
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

vector<PqlToken> valid_pattern = {
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
	PqlToken(TokenType::SYNONYM, "v"),
	PqlToken(TokenType::PATTERN, "pattern"),
	PqlToken(TokenType::SYNONYM, "a"),
	PqlToken(TokenType::OPEN_BRACKET, "("),
	PqlToken(TokenType::SYNONYM, "v"),
	PqlToken(TokenType::COMMA, ","),
	PqlToken(TokenType::WILDCARD, "_"),
	PqlToken(TokenType::CLOSED_BRACKET, ")")
};

vector<PqlToken> valid_uses = {
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
	PqlToken(TokenType::SYNONYM, "v"),
	PqlToken(TokenType::SUCH, "such"),
	PqlToken(TokenType::THAT, "that"),
	PqlToken(TokenType::USES, "uses"),
	PqlToken(TokenType::OPEN_BRACKET, "("),
	PqlToken(TokenType::STATEMENT_NUM, "1"),
	PqlToken(TokenType::COMMA, ","),
	PqlToken(TokenType::SYNONYM, "v"),
	PqlToken(TokenType::CLOSED_BRACKET, ")")
};

vector<PqlToken> valid_modifies = {
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
	PqlToken(TokenType::SYNONYM, "v"),
	PqlToken(TokenType::SUCH, "such"),
	PqlToken(TokenType::THAT, "that"),
	PqlToken(TokenType::MODIFIES, "modifies"),
	PqlToken(TokenType::OPEN_BRACKET, "("),
	PqlToken(TokenType::STATEMENT_NUM, "1"),
	PqlToken(TokenType::COMMA, ","),
	PqlToken(TokenType::SYNONYM, "v"),
	PqlToken(TokenType::CLOSED_BRACKET, ")")
};

vector<PqlToken> valid_follows = {
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
	PqlToken(TokenType::SYNONYM, "v"),
	PqlToken(TokenType::SUCH, "such"),
	PqlToken(TokenType::THAT, "that"),
	PqlToken(TokenType::FOLLOWS, "follows"),
	PqlToken(TokenType::OPEN_BRACKET, "("),
	PqlToken(TokenType::SYNONYM, "s"),
	PqlToken(TokenType::COMMA, ","),
	PqlToken(TokenType::STATEMENT_NUM, "1"),
	PqlToken(TokenType::CLOSED_BRACKET, ")")
};

vector<PqlToken> valid_parent = {
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
	PqlToken(TokenType::SYNONYM, "v"),
	PqlToken(TokenType::SUCH, "such"),
	PqlToken(TokenType::THAT, "that"),
	PqlToken(TokenType::PARENT, "parent"),
	PqlToken(TokenType::OPEN_BRACKET, "("),
	PqlToken(TokenType::SYNONYM, "s"),
	PqlToken(TokenType::COMMA, ","),
	PqlToken(TokenType::STATEMENT_NUM, "1"),
	PqlToken(TokenType::CLOSED_BRACKET, ")")
};

vector<PqlToken> valid_calls = {
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
	PqlToken(TokenType::SYNONYM, "v"),
	PqlToken(TokenType::SUCH, "such"),
	PqlToken(TokenType::THAT, "that"),
	PqlToken(TokenType::CALLS, "calls"),
	PqlToken(TokenType::OPEN_BRACKET, "("),
	PqlToken(TokenType::SYNONYM, "v"),
	PqlToken(TokenType::COMMA, ","),
	PqlToken(TokenType::WILDCARD, "_"),
	PqlToken(TokenType::CLOSED_BRACKET, ")")
};

vector<PqlToken> valid_follows_a = {
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
	PqlToken(TokenType::SYNONYM, "v"),
	PqlToken(TokenType::SUCH, "such"),
	PqlToken(TokenType::THAT, "that"),
	PqlToken(TokenType::FOLLOWS_A, "follows*"),
	PqlToken(TokenType::OPEN_BRACKET, "("),
	PqlToken(TokenType::SYNONYM, "s"),
	PqlToken(TokenType::COMMA, ","),
	PqlToken(TokenType::STATEMENT_NUM, "1"),
	PqlToken(TokenType::CLOSED_BRACKET, ")")
};

vector<PqlToken> valid_parent_a = {
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
	PqlToken(TokenType::SYNONYM, "v"),
	PqlToken(TokenType::SUCH, "such"),
	PqlToken(TokenType::THAT, "that"),
	PqlToken(TokenType::PARENT_A, "parent*"),
	PqlToken(TokenType::OPEN_BRACKET, "("),
	PqlToken(TokenType::SYNONYM, "s"),
	PqlToken(TokenType::COMMA, ","),
	PqlToken(TokenType::STATEMENT_NUM, "1"),
	PqlToken(TokenType::CLOSED_BRACKET, ")")
};

vector<PqlToken> valid_calls_a = {
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
	PqlToken(TokenType::SYNONYM, "v"),
	PqlToken(TokenType::SUCH, "such"),
	PqlToken(TokenType::THAT, "that"),
	PqlToken(TokenType::CALLS_A, "calls*"),
	PqlToken(TokenType::OPEN_BRACKET, "("),
	PqlToken(TokenType::SYNONYM, "v"),
	PqlToken(TokenType::COMMA, ","),
	PqlToken(TokenType::WILDCARD, "_"),
	PqlToken(TokenType::CLOSED_BRACKET, ")")
};

vector<PqlToken> valid_pattern_then_such_that = {
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
	PqlToken(TokenType::SYNONYM, "v"),
	PqlToken(TokenType::PATTERN, "pattern"),
	PqlToken(TokenType::SYNONYM, "a"),
	PqlToken(TokenType::OPEN_BRACKET, "("),
	PqlToken(TokenType::SYNONYM, "v"),
	PqlToken(TokenType::COMMA, ","),
	PqlToken(TokenType::WILDCARD, "_"),
	PqlToken(TokenType::CLOSED_BRACKET, ")"),
	PqlToken(TokenType::SUCH, "such"),
	PqlToken(TokenType::THAT, "that"),
	PqlToken(TokenType::USES, "uses"),
	PqlToken(TokenType::OPEN_BRACKET, "("),
	PqlToken(TokenType::STATEMENT_NUM, "1"),
	PqlToken(TokenType::COMMA, ","),
	PqlToken(TokenType::SYNONYM, "v"),
	PqlToken(TokenType::CLOSED_BRACKET, ")")
};

vector<PqlToken> valid_such_that_then_pattern = {
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
	PqlToken(TokenType::SYNONYM, "v"),
	PqlToken(TokenType::SUCH, "such"),
	PqlToken(TokenType::THAT, "that"),
	PqlToken(TokenType::USES, "uses"),
	PqlToken(TokenType::OPEN_BRACKET, "("),
	PqlToken(TokenType::STATEMENT_NUM, "1"),
	PqlToken(TokenType::COMMA, ","),
	PqlToken(TokenType::SYNONYM, "v"),
	PqlToken(TokenType::CLOSED_BRACKET, ")"),
	PqlToken(TokenType::PATTERN, "pattern"),
	PqlToken(TokenType::SYNONYM, "a"),
	PqlToken(TokenType::OPEN_BRACKET, "("),
	PqlToken(TokenType::SYNONYM, "v"),
	PqlToken(TokenType::COMMA, ","),
	PqlToken(TokenType::WILDCARD, "_"),
	PqlToken(TokenType::CLOSED_BRACKET, ")"),
};

vector<PqlToken> valid_multi_pattern_then_multi_such_that = {
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
	PqlToken(TokenType::SYNONYM, "v"),
	PqlToken(TokenType::PATTERN, "pattern"),
	PqlToken(TokenType::SYNONYM, "a"),
	PqlToken(TokenType::OPEN_BRACKET, "("),
	PqlToken(TokenType::SYNONYM, "v"),
	PqlToken(TokenType::COMMA, ","),
	PqlToken(TokenType::WILDCARD, "_"),
	PqlToken(TokenType::CLOSED_BRACKET, ")"),
	PqlToken(TokenType::AND, "and"),
	PqlToken(TokenType::SYNONYM, "a"),
	PqlToken(TokenType::OPEN_BRACKET, "("),
	PqlToken(TokenType::SYNONYM, "v"),
	PqlToken(TokenType::COMMA, ","),
	PqlToken(TokenType::WILDCARD, "_"),
	PqlToken(TokenType::CLOSED_BRACKET, ")"),
	PqlToken(TokenType::SUCH, "such"),
	PqlToken(TokenType::THAT, "that"),
	PqlToken(TokenType::USES, "uses"),
	PqlToken(TokenType::OPEN_BRACKET, "("),
	PqlToken(TokenType::STATEMENT_NUM, "1"),
	PqlToken(TokenType::COMMA, ","),
	PqlToken(TokenType::SYNONYM, "v"),
	PqlToken(TokenType::CLOSED_BRACKET, ")"),
	PqlToken(TokenType::AND, "and"),
	PqlToken(TokenType::MODIFIES, "modifies"),
	PqlToken(TokenType::OPEN_BRACKET, "("),
	PqlToken(TokenType::STATEMENT_NUM, "1"),
	PqlToken(TokenType::COMMA, ","),
	PqlToken(TokenType::SYNONYM, "v"),
	PqlToken(TokenType::CLOSED_BRACKET, ")"),
};


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
	PqlToken(TokenType::CLOSED_BRACKET, ")"),
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

vector<PqlToken> invalid_wildcard_uses = {
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
	PqlToken(TokenType::SYNONYM, "v"),
	PqlToken(TokenType::SUCH, "such"),
	PqlToken(TokenType::THAT, "that"),
	PqlToken(TokenType::USES, "uses"),
	PqlToken(TokenType::OPEN_BRACKET, "("),
	PqlToken(TokenType::WILDCARD, "_"),
	PqlToken(TokenType::COMMA, ","),
	PqlToken(TokenType::SYNONYM, "v"),
	PqlToken(TokenType::CLOSED_BRACKET, ")")
};

vector<PqlToken> invalid_wildcard_modifies = {
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
	PqlToken(TokenType::SYNONYM, "v"),
	PqlToken(TokenType::SUCH, "such"),
	PqlToken(TokenType::THAT, "that"),
	PqlToken(TokenType::MODIFIES, "modifies"),
	PqlToken(TokenType::OPEN_BRACKET, "("),
	PqlToken(TokenType::WILDCARD, "_"),
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