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

vector<PqlToken> valid_select_only = {
	PqlToken(TokenType::DECLARATION_END, ""),
	PqlToken(TokenType::SELECT, "select"),
	PqlToken(TokenType::BOOLEAN, "BOOLEAN"),
	PqlToken(TokenType::WITH, "with"),
	PqlToken(TokenType::NUMBER, "12"),
	PqlToken(TokenType::EQUAL, "="),
	PqlToken(TokenType::NUMBER, "12")
};

vector<PqlToken> valid_calls_wildcards = {
	PqlToken(TokenType::DECLARATION_END, ""),
	PqlToken(TokenType::SELECT, "select"),
	PqlToken(TokenType::BOOLEAN, "BOOLEAN"),
	PqlToken(TokenType::SUCH, "such"),
	PqlToken(TokenType::THAT, "that"),
	PqlToken(TokenType::CALLS, "Calls"),
	PqlToken(TokenType::OPEN_BRACKET, "("),
	PqlToken(TokenType::WILDCARD, "_"),
	PqlToken(TokenType::COMMA, ","),
	PqlToken(TokenType::WILDCARD, "_"),
	PqlToken(TokenType::CLOSED_BRACKET, ")")
};

vector<PqlToken> valid_select_boolean = {
	PqlToken(TokenType::VARIABLE, "variable"),
	PqlToken(TokenType::SYNONYM, "v"),
	PqlToken(TokenType::SEMICOLON, ";"),
	PqlToken(TokenType::DECLARATION_END, ""),
	PqlToken(TokenType::SELECT, "select"),
	PqlToken(TokenType::BOOLEAN, "BOOLEAN")
};

vector<PqlToken> valid_select_declared_boolean = {
	PqlToken(TokenType::VARIABLE, "variable"),
	PqlToken(TokenType::SYNONYM, "BOOLEAN"),
	PqlToken(TokenType::SEMICOLON, ";"),
	PqlToken(TokenType::DECLARATION_END, ""),
	PqlToken(TokenType::SELECT, "select"),
	PqlToken(TokenType::BOOLEAN, "BOOLEAN")
};

vector<PqlToken> valid_select_declared_boolean_attrname = {
	PqlToken(TokenType::STATEMENT, "stmt"),
	PqlToken(TokenType::SYNONYM, "BOOLEAN"),
	PqlToken(TokenType::SEMICOLON, ";"),
	PqlToken(TokenType::DECLARATION_END, ""),
	PqlToken(TokenType::SELECT, "select"),
	PqlToken(TokenType::BOOLEAN, "BOOLEAN"),
	PqlToken(TokenType::DOT, "."),
	PqlToken(TokenType::STMTLINE, "stmt#")
};

vector<PqlToken> valid_select_attrname = {
	PqlToken(TokenType::STATEMENT, "stmt"),
	PqlToken(TokenType::SYNONYM, "s"),
	PqlToken(TokenType::SEMICOLON, ";"),
	PqlToken(TokenType::DECLARATION_END, ""),
	PqlToken(TokenType::SELECT, "select"),
	PqlToken(TokenType::SYNONYM, "s"),
	PqlToken(TokenType::DOT, "."),
	PqlToken(TokenType::STMTLINE, "stmt#")
};

vector<PqlToken> valid_select_tuple = {
	PqlToken(TokenType::VARIABLE, "variable"),
	PqlToken(TokenType::SYNONYM, "v"),
	PqlToken(TokenType::SEMICOLON, ";"),
	PqlToken(TokenType::STATEMENT, "stmt"),
	PqlToken(TokenType::SYNONYM, "BOOLEAN"),
	PqlToken(TokenType::SEMICOLON, ";"),
	PqlToken(TokenType::DECLARATION_END, ""),
	PqlToken(TokenType::SELECT, "select"),
	PqlToken(TokenType::OPEN_ARROW, "<"),
	PqlToken(TokenType::SYNONYM, "BOOLEAN"),
	PqlToken(TokenType::DOT, "."),
	PqlToken(TokenType::STMTLINE, "stmt#"),
	PqlToken(TokenType::COMMA, ","),
	PqlToken(TokenType::SYNONYM, "v"),
	PqlToken(TokenType::CLOSED_ARROW, ">")
};

vector<PqlToken> invalid_select_missing_arrows = {
	PqlToken(TokenType::VARIABLE, "variable"),
	PqlToken(TokenType::SYNONYM, "v"),
	PqlToken(TokenType::SEMICOLON, ";"),
	PqlToken(TokenType::STATEMENT, "stmt"),
	PqlToken(TokenType::SYNONYM, "s"),
	PqlToken(TokenType::SEMICOLON, ";"),
	PqlToken(TokenType::DECLARATION_END, ""),
	PqlToken(TokenType::SELECT, "select"),
	PqlToken(TokenType::SYNONYM, "s"),
	PqlToken(TokenType::DOT, "."),
	PqlToken(TokenType::STMTLINE, "stmt#"),
	PqlToken(TokenType::COMMA, ","),
	PqlToken(TokenType::SYNONYM, "v"),
	PqlToken(TokenType::CLOSED_ARROW, ">")
};

vector<PqlToken> invalid_select_missing_comma = {
	PqlToken(TokenType::VARIABLE, "variable"),
	PqlToken(TokenType::SYNONYM, "v"),
	PqlToken(TokenType::SEMICOLON, ";"),
	PqlToken(TokenType::STATEMENT, "stmt"),
	PqlToken(TokenType::SYNONYM, "s"),
	PqlToken(TokenType::SEMICOLON, ";"),
	PqlToken(TokenType::DECLARATION_END, ""),
	PqlToken(TokenType::SELECT, "select"),
	PqlToken(TokenType::SYNONYM, "s"),
	PqlToken(TokenType::DOT, "."),
	PqlToken(TokenType::STMTLINE, "stmt#"),
	PqlToken(TokenType::SYNONYM, "v"),
	PqlToken(TokenType::CLOSED_ARROW, ">")
};

vector<PqlToken> valid_pattern_assign = {
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

vector<PqlToken> valid_pattern_assign_boolean = {
	PqlToken(TokenType::VARIABLE, "variable"),
	PqlToken(TokenType::SYNONYM, "v"),
	PqlToken(TokenType::SEMICOLON, ";"),
	PqlToken(TokenType::ASSIGN, "assign"),
	PqlToken(TokenType::SYNONYM, "BOOLEAN"),
	PqlToken(TokenType::SEMICOLON, ";"),
	PqlToken(TokenType::ASSIGN, "statement"),
	PqlToken(TokenType::SYNONYM, "s"),
	PqlToken(TokenType::SEMICOLON, ";"),
	PqlToken(TokenType::DECLARATION_END, ""),
	PqlToken(TokenType::SELECT, "select"),
	PqlToken(TokenType::SYNONYM, "v"),
	PqlToken(TokenType::PATTERN, "pattern"),
	PqlToken(TokenType::BOOLEAN, "BOOLEAN"),
	PqlToken(TokenType::OPEN_BRACKET, "("),
	PqlToken(TokenType::SYNONYM, "v"),
	PqlToken(TokenType::COMMA, ","),
	PqlToken(TokenType::WILDCARD, "_"),
	PqlToken(TokenType::CLOSED_BRACKET, ")")
};

vector<PqlToken> valid_pattern_while = {
	PqlToken(TokenType::VARIABLE, "variable"),
	PqlToken(TokenType::SYNONYM, "v"),
	PqlToken(TokenType::SEMICOLON, ";"),
	PqlToken(TokenType::WHILE, "while"),
	PqlToken(TokenType::SYNONYM, "w"),
	PqlToken(TokenType::SEMICOLON, ";"),
	PqlToken(TokenType::DECLARATION_END, ""),
	PqlToken(TokenType::SELECT, "select"),
	PqlToken(TokenType::SYNONYM, "w"),
	PqlToken(TokenType::PATTERN, "pattern"),
	PqlToken(TokenType::SYNONYM, "w"),
	PqlToken(TokenType::OPEN_BRACKET, "("),
	PqlToken(TokenType::SYNONYM, "v"),
	PqlToken(TokenType::COMMA, ","),
	PqlToken(TokenType::WILDCARD, "_"),
	PqlToken(TokenType::CLOSED_BRACKET, ")")
};

vector<PqlToken> valid_pattern_if = {
	PqlToken(TokenType::VARIABLE, "variable"),
	PqlToken(TokenType::SYNONYM, "v"),
	PqlToken(TokenType::SEMICOLON, ";"),
	PqlToken(TokenType::IF, "if"),
	PqlToken(TokenType::SYNONYM, "ifs"),
	PqlToken(TokenType::SEMICOLON, ";"),
	PqlToken(TokenType::DECLARATION_END, ""),
	PqlToken(TokenType::SELECT, "select"),
	PqlToken(TokenType::SYNONYM, "v"),
	PqlToken(TokenType::PATTERN, "pattern"),
	PqlToken(TokenType::SYNONYM, "ifs"),
	PqlToken(TokenType::OPEN_BRACKET, "("),
	PqlToken(TokenType::SYNONYM, "v"),
	PqlToken(TokenType::COMMA, ","),
	PqlToken(TokenType::WILDCARD, "_"),
	PqlToken(TokenType::COMMA, ","),
	PqlToken(TokenType::WILDCARD, "_"),
	PqlToken(TokenType::CLOSED_BRACKET, ")")
};

vector<PqlToken> invalid_pattern_while_parameters = {
	PqlToken(TokenType::VARIABLE, "variable"),
	PqlToken(TokenType::SYNONYM, "v"),
	PqlToken(TokenType::SEMICOLON, ";"),
	PqlToken(TokenType::WHILE, "while"),
	PqlToken(TokenType::SYNONYM, "w"),
	PqlToken(TokenType::SEMICOLON, ";"),
	PqlToken(TokenType::DECLARATION_END, ""),
	PqlToken(TokenType::SELECT, "select"),
	PqlToken(TokenType::SYNONYM, "w"),
	PqlToken(TokenType::PATTERN, "pattern"),
	PqlToken(TokenType::SYNONYM, "w"),
	PqlToken(TokenType::OPEN_BRACKET, "("),
	PqlToken(TokenType::SYNONYM, "v"),
	PqlToken(TokenType::COMMA, ","),
	PqlToken(TokenType::WILDCARD_STRING, "_\"x\"_"),
	PqlToken(TokenType::CLOSED_BRACKET, ")")
};

vector<PqlToken> invalid_pattern_if_parameters = {
	PqlToken(TokenType::VARIABLE, "variable"),
	PqlToken(TokenType::SYNONYM, "v"),
	PqlToken(TokenType::SEMICOLON, ";"),
	PqlToken(TokenType::IF, "if"),
	PqlToken(TokenType::SYNONYM, "ifs"),
	PqlToken(TokenType::SEMICOLON, ";"),
	PqlToken(TokenType::DECLARATION_END, ""),
	PqlToken(TokenType::SELECT, "select"),
	PqlToken(TokenType::SYNONYM, "v"),
	PqlToken(TokenType::PATTERN, "pattern"),
	PqlToken(TokenType::SYNONYM, "ifs"),
	PqlToken(TokenType::OPEN_BRACKET, "("),
	PqlToken(TokenType::SYNONYM, "v"),
	PqlToken(TokenType::COMMA, ","),
	PqlToken(TokenType::WILDCARD, "_"),
	PqlToken(TokenType::CLOSED_BRACKET, ")")
};

vector<PqlToken> valid_pattern_multi = {
	PqlToken(TokenType::VARIABLE, "variable"),
	PqlToken(TokenType::SYNONYM, "v"),
	PqlToken(TokenType::SEMICOLON, ";"),
	PqlToken(TokenType::IF, "if"),
	PqlToken(TokenType::SYNONYM, "ifs"),
	PqlToken(TokenType::SEMICOLON, ";"),
	PqlToken(TokenType::WHILE, "while"),
	PqlToken(TokenType::SYNONYM, "w"),
	PqlToken(TokenType::SEMICOLON, ";"),
	PqlToken(TokenType::DECLARATION_END, ""),
	PqlToken(TokenType::SELECT, "select"),
	PqlToken(TokenType::SYNONYM, "v"),
	PqlToken(TokenType::PATTERN, "pattern"),
	PqlToken(TokenType::SYNONYM, "ifs"),
	PqlToken(TokenType::OPEN_BRACKET, "("),
	PqlToken(TokenType::SYNONYM, "v"),
	PqlToken(TokenType::COMMA, ","),
	PqlToken(TokenType::WILDCARD, "_"),
	PqlToken(TokenType::COMMA, ","),
	PqlToken(TokenType::WILDCARD, "_"),
	PqlToken(TokenType::CLOSED_BRACKET, ")"),
	PqlToken(TokenType::AND, "and"),
	PqlToken(TokenType::SYNONYM, "w"),
	PqlToken(TokenType::OPEN_BRACKET, "("),
	PqlToken(TokenType::SYNONYM, "v"),
	PqlToken(TokenType::COMMA, ","),
	PqlToken(TokenType::WILDCARD, "_"),
	PqlToken(TokenType::CLOSED_BRACKET, ")")
};


vector<PqlToken> valid_pattern_with_string = {
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
	PqlToken(TokenType::SYNONYM, "v"),
	PqlToken(TokenType::COMMA, ","),
	PqlToken(TokenType::STRING, "\"x+y\""),
	PqlToken(TokenType::CLOSED_BRACKET, ")")
};

vector<PqlToken> valid_pattern_with_invalid_string = {
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
	PqlToken(TokenType::SYNONYM, "v"),
	PqlToken(TokenType::COMMA, ","),
	PqlToken(TokenType::STRING, "\"+x\""),
	PqlToken(TokenType::CLOSED_BRACKET, ")")
};


vector<PqlToken> valid_pattern_with_string_and_whitespace = {
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
	PqlToken(TokenType::SYNONYM, "v"),
	PqlToken(TokenType::COMMA, ","),
	PqlToken(TokenType::STRING, "\" x + y   \""),
	PqlToken(TokenType::CLOSED_BRACKET, ")")
};

vector<PqlToken> valid_pattern_with_wildcard_string = {
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
	PqlToken(TokenType::SYNONYM, "v"),
	PqlToken(TokenType::COMMA, ","),
	PqlToken(TokenType::WILDCARD_STRING, "_\"x+y\"_"),
	PqlToken(TokenType::CLOSED_BRACKET, ")")
};

vector<PqlToken> invalid_pattern_wildcard_string = {
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
	PqlToken(TokenType::SYNONYM, "v"),
	PqlToken(TokenType::COMMA, ","),
	PqlToken(TokenType::WILDCARD_STRING, "\"x+y\"_"),
	PqlToken(TokenType::CLOSED_BRACKET, ")")
};


vector<PqlToken> valid_follows_double_wildcard = {
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
	PqlToken(TokenType::WILDCARD,"_"),
	PqlToken(TokenType::COMMA, ","),
	PqlToken(TokenType::WILDCARD, "_"),
	PqlToken(TokenType::CLOSED_BRACKET, ")")
};

vector<PqlToken> valid_with = {
	PqlToken(TokenType::PROCEDURE, "procedure"),
	PqlToken(TokenType::SYNONYM, "p"),
	PqlToken(TokenType::SEMICOLON, ";"),
	PqlToken(TokenType::DECLARATION_END, ""),
	PqlToken(TokenType::SELECT, "select"),
	PqlToken(TokenType::SYNONYM, "p"),
	PqlToken(TokenType::WITH, "with"),
	PqlToken(TokenType::SYNONYM, "p"),
	PqlToken(TokenType::DOT, "."),
	PqlToken(TokenType::PROCNAME, "procName"),
	PqlToken(TokenType::EQUAL, "="),
	PqlToken(TokenType::STRING, "\"answer\"")
};

vector<PqlToken> invalid_with_attrname_mismatch = {
	PqlToken(TokenType::PROCEDURE, "procedure"),
	PqlToken(TokenType::SYNONYM, "p"),
	PqlToken(TokenType::SEMICOLON, ";"),
	PqlToken(TokenType::DECLARATION_END, ""),
	PqlToken(TokenType::SELECT, "select"),
	PqlToken(TokenType::SYNONYM, "p"),
	PqlToken(TokenType::WITH, "with"),
	PqlToken(TokenType::SYNONYM, "p"),
	PqlToken(TokenType::DOT, "."),
	PqlToken(TokenType::VARNAME, "var"),
	PqlToken(TokenType::EQUAL, "="),
	PqlToken(TokenType::STRING, "\"answer\"")
};

vector<PqlToken> valid_multi_with = {
	PqlToken(TokenType::PROCEDURE, "procedure"),
	PqlToken(TokenType::SYNONYM, "p"),
	PqlToken(TokenType::SEMICOLON, ";"),
	PqlToken(TokenType::DECLARATION_END, ""),
	PqlToken(TokenType::SELECT, "select"),
	PqlToken(TokenType::SYNONYM, "p"),
	PqlToken(TokenType::WITH, "with"),
	PqlToken(TokenType::SYNONYM, "p"),
	PqlToken(TokenType::DOT, "."),
	PqlToken(TokenType::PROCNAME, "procName"),
	PqlToken(TokenType::EQUAL, "="),
	PqlToken(TokenType::STRING, "\" answer\""),
	PqlToken(TokenType::AND, "and"),
	PqlToken(TokenType::STRING, "\"answer2\""),
	PqlToken(TokenType::EQUAL, "="),
	PqlToken(TokenType::SYNONYM, "p"),
	PqlToken(TokenType::DOT, "."),
	PqlToken(TokenType::PROCNAME, "procName"),
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

vector<PqlToken> valid_multi_pattern_with_such_that = {
	PqlToken(TokenType::VARIABLE, "variable"),
	PqlToken(TokenType::SYNONYM, "v"),
	PqlToken(TokenType::SEMICOLON, ";"),
	PqlToken(TokenType::PROCEDURE, "procedure"),
	PqlToken(TokenType::SYNONYM, "p"),
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
	PqlToken(TokenType::WITH, "with"),
	PqlToken(TokenType::SYNONYM, "p"),
	PqlToken(TokenType::DOT, "."),
	PqlToken(TokenType::PROCNAME, "procName"),
	PqlToken(TokenType::EQUAL, "="),
	PqlToken(TokenType::STRING, "\"one\""),
	PqlToken(TokenType::AND, "and"),
	PqlToken(TokenType::STRING, "\"two\""),
	PqlToken(TokenType::EQUAL, "="),
	PqlToken(TokenType::SYNONYM, "p"),
	PqlToken(TokenType::DOT, "."),
	PqlToken(TokenType::PROCNAME, "procName"),
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

vector<PqlToken> invalid_with_ref_mismatch = {
	PqlToken(TokenType::PROCEDURE, "procedure"),
	PqlToken(TokenType::SYNONYM, "p"),
	PqlToken(TokenType::SEMICOLON, ";"),
	PqlToken(TokenType::DECLARATION_END, ""),
	PqlToken(TokenType::SELECT, "select"),
	PqlToken(TokenType::SYNONYM, "p"),
	PqlToken(TokenType::WITH, "with"),
	PqlToken(TokenType::SYNONYM, "p"),
	PqlToken(TokenType::DOT, "."),
	PqlToken(TokenType::VARNAME, "var"),
	PqlToken(TokenType::EQUAL, "="),
	PqlToken(TokenType::NUMBER, "2")
};

vector<PqlToken> invalid_with_parameter_type = {
	PqlToken(TokenType::PROCEDURE, "procedure"),
	PqlToken(TokenType::SYNONYM, "p"),
	PqlToken(TokenType::SEMICOLON, ";"),
	PqlToken(TokenType::DECLARATION_END, ""),
	PqlToken(TokenType::SELECT, "select"),
	PqlToken(TokenType::SYNONYM, "p"),
	PqlToken(TokenType::WITH, "with"),
	PqlToken(TokenType::SYNONYM, "p"),
	PqlToken(TokenType::DOT, "."),
	PqlToken(TokenType::VARNAME, "var"),
	PqlToken(TokenType::EQUAL, "="),
	PqlToken(TokenType::WILDCARD, "_")
};

vector<PqlToken> invalid_with_undeclared_synonym = {
	PqlToken(TokenType::PROCEDURE, "procedure"),
	PqlToken(TokenType::SYNONYM, "p"),
	PqlToken(TokenType::SEMICOLON, ";"),
	PqlToken(TokenType::DECLARATION_END, ""),
	PqlToken(TokenType::SELECT, "select"),
	PqlToken(TokenType::SYNONYM, "p"),
	PqlToken(TokenType::WITH, "with"),
	PqlToken(TokenType::SYNONYM, "p2"),
	PqlToken(TokenType::DOT, "."),
	PqlToken(TokenType::VARNAME, "var"),
	PqlToken(TokenType::EQUAL, "="),
	PqlToken(TokenType::STRING, "answer")
};

vector<PqlToken> invalid_with_synonym_attrname = {
	PqlToken(TokenType::PROCEDURE, "procedure"),
	PqlToken(TokenType::SYNONYM, "p"),
	PqlToken(TokenType::SEMICOLON, ";"),
	PqlToken(TokenType::DECLARATION_END, ""),
	PqlToken(TokenType::SELECT, "select"),
	PqlToken(TokenType::SYNONYM, "p"),
	PqlToken(TokenType::WITH, "with"),
	PqlToken(TokenType::SYNONYM, "p"),
	PqlToken(TokenType::DOT, "."),
	PqlToken(TokenType::STRING, "var"),
	PqlToken(TokenType::EQUAL, "="),
	PqlToken(TokenType::STRING, "answer")
};

vector<PqlToken> invalid_with_missing_dot = {
	PqlToken(TokenType::PROCEDURE, "procedure"),
	PqlToken(TokenType::SYNONYM, "p"),
	PqlToken(TokenType::SEMICOLON, ";"),
	PqlToken(TokenType::DECLARATION_END, ""),
	PqlToken(TokenType::SELECT, "select"),
	PqlToken(TokenType::SYNONYM, "p"),
	PqlToken(TokenType::WITH, "with"),
	PqlToken(TokenType::SYNONYM, "p"),
	PqlToken(TokenType::VARNAME, "var"),
	PqlToken(TokenType::EQUAL, "="),
	PqlToken(TokenType::STRING, "answer")
};

vector<PqlToken> invalid_with_parameter_extra_tokens = {
	PqlToken(TokenType::PROCEDURE, "procedure"),
	PqlToken(TokenType::SYNONYM, "p"),
	PqlToken(TokenType::SEMICOLON, ";"),
	PqlToken(TokenType::DECLARATION_END, ""),
	PqlToken(TokenType::SELECT, "select"),
	PqlToken(TokenType::SYNONYM, "p"),
	PqlToken(TokenType::WITH, "with"),
	PqlToken(TokenType::SYNONYM, "p"),
	PqlToken(TokenType::DOT, "."),
	PqlToken(TokenType::VARNAME, "var"),
	PqlToken(TokenType::VARNAME, "var"),
	PqlToken(TokenType::VARNAME, "var"),
	PqlToken(TokenType::EQUAL, "="),
	PqlToken(TokenType::STRING, "answer")
};
