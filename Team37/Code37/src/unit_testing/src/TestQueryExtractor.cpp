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
#include "QPS/Validators/ValidatorUtils.h"
using namespace std;

bool isSameMap(unordered_map<string, TokenType> ans, unordered_map<string, TokenType> result) {
	for (const auto & [key, value] : ans) {
		if (result.find(key) == result.end()) {
			return false;
		}
		if (result[key] != value) {
			return false;
		}
	}
	return true;
}

bool isSameClauses(vector<Clause> ans, vector<Clause> result)
{
	if (ans.size() != result.size())
	{
		return false;
	}

	for (int i = 0; i != ans.size(); i++)
	{
		if (ans[i] == result[i]) {
			continue;
		}
		else
		{
			return false;
		}
	}

	return true;
}


TEST_CASE("Test declartions")
{
	unordered_map<string, TokenType> ans = {
		{"v", TokenType::VARIABLE},
		{"a", TokenType::ASSIGN}
	};

	QueryExtractor sut(basic_tokens);
	PqlQuery results = sut.extractSemantics();

	REQUIRE(isSameMap(ans, results.declarations));
}

TEST_CASE("Test Select clause")
{
	vector<SelectObject> ans = { SelectObject(SelectType::SYNONYM, "v") };

	QueryExtractor sut(basic_tokens);
	PqlQuery results = sut.extractSemantics();

	REQUIRE(results.selectObjects == ans);
}

TEST_CASE("Test Select boolean")
{
	vector<SelectObject> ans = { SelectObject(SelectType::BOOLEAN) };

	QueryExtractor sut(valid_select_boolean);
	PqlQuery results = sut.extractSemantics();

	REQUIRE(results.selectObjects == ans);
}

TEST_CASE("Test Select declared boolean")
{
	vector<SelectObject> ans = { SelectObject(SelectType::SYNONYM, "BOOLEAN")};

	QueryExtractor sut(valid_select_declared_boolean);
	PqlQuery results = sut.extractSemantics();

	REQUIRE(results.selectObjects == ans);
}

TEST_CASE("Test Select attrName")
{
	vector<SelectObject> ans = { SelectObject(SelectType::ATTRNAME, "s", PqlToken(TokenType::STMTLINE, "stmt#"))};

	QueryExtractor sut(valid_select_attrname);
	PqlQuery results = sut.extractSemantics();

	REQUIRE(results.selectObjects == ans);
}

TEST_CASE("Test Select tuple")
{
	vector<SelectObject> ans = { 
		SelectObject(SelectType::ATTRNAME, "BOOLEAN", PqlToken(TokenType::STMTLINE, "stmt#")),
		SelectObject(SelectType::SYNONYM, "v")
	};

	QueryExtractor sut(valid_select_tuple);
	PqlQuery results = sut.extractSemantics();

	REQUIRE(results.selectObjects == ans);
}


TEST_CASE("Test Pattern clause assign")
{
	vector<Clause> ans = { Clause(PqlToken(TokenType::SYNONYM, "a"),
								  PqlToken(TokenType::SYNONYM, "v"),
								  PqlToken(TokenType::WILDCARD, "_"),
								  TokenType::PATTERN)};

	QueryExtractor sut(valid_pattern_assign);
	PqlQuery results = sut.extractSemantics();

	REQUIRE(isSameClauses(ans, results.clauses));
}

TEST_CASE("Test Pattern clause while")
{
	vector<Clause> ans = { Clause(PqlToken(TokenType::SYNONYM, "w"),
								  PqlToken(TokenType::SYNONYM, "v"),
								  PqlToken(TokenType::WILDCARD, "_"),
								  TokenType::PATTERN) };

	QueryExtractor sut(valid_pattern_while);
	PqlQuery results = sut.extractSemantics();

	REQUIRE(isSameClauses(ans, results.clauses));
}

TEST_CASE("Test Pattern clause if")
{
	vector<Clause> ans = { Clause(PqlToken(TokenType::SYNONYM, "ifs"),
								  PqlToken(TokenType::SYNONYM, "v"),
								  PqlToken(TokenType::WILDCARD, "_"),
								  TokenType::PATTERN) };

	QueryExtractor sut(valid_pattern_if);
	PqlQuery results = sut.extractSemantics();

	REQUIRE(isSameClauses(ans, results.clauses));
}

TEST_CASE("Test Pattern clause multi")
{
	vector<Clause> ans = { 
		Clause(PqlToken(TokenType::SYNONYM, "ifs"),
								  PqlToken(TokenType::SYNONYM, "v"),
								  PqlToken(TokenType::WILDCARD, "_"),
								  TokenType::PATTERN),
		Clause(PqlToken(TokenType::SYNONYM, "w"),
								  PqlToken(TokenType::SYNONYM, "v"),
								  PqlToken(TokenType::WILDCARD, "_"),
								  TokenType::PATTERN)
	};

	QueryExtractor sut(valid_pattern_multi);
	PqlQuery results = sut.extractSemantics();

	REQUIRE(isSameClauses(ans, results.clauses));
}


TEST_CASE("Test Pattern clause with String")
{
	vector<Clause> ans = { Clause(PqlToken(TokenType::SYNONYM, "a"),
								  PqlToken(TokenType::SYNONYM, "v"),
								  PqlToken(TokenType::STRING, "x+y"),
								  TokenType::PATTERN) };

	QueryExtractor sut(valid_pattern_with_string);
	PqlQuery results = sut.extractSemantics();

	REQUIRE(isSameClauses(ans, results.clauses));
}

TEST_CASE("Test Pattern clause with String and whitespace")
{
	vector<Clause> ans = { Clause(PqlToken(TokenType::SYNONYM, "a"),
								  PqlToken(TokenType::SYNONYM, "v"),
								  PqlToken(TokenType::STRING, "x+y"),
								  TokenType::PATTERN) };

	QueryExtractor sut(valid_pattern_with_string_and_whitespace);
	PqlQuery results = sut.extractSemantics();

	REQUIRE(isSameClauses(ans, results.clauses));
}

TEST_CASE("Test Pattern clause with Wildcard String")
{
	vector<Clause> ans = { Clause(PqlToken(TokenType::SYNONYM, "a"),
								  PqlToken(TokenType::SYNONYM, "v"),
								  PqlToken(TokenType::WILDCARD_STRING, "x+y"),
								  TokenType::PATTERN) };

	QueryExtractor sut(valid_pattern_with_wildcard_string);
	PqlQuery results = sut.extractSemantics();
	//REQUIRE("x+y" == results.clauses[0].right.value);
	REQUIRE(isSameClauses(ans, results.clauses));
}


TEST_CASE("Test With clause")
{
	vector<Clause> ans = { Clause(PqlToken(TokenType::NONE, ""),
								  PqlToken(TokenType::SYNONYM, "p"),
								  PqlToken(TokenType::STRING, "answer"),
								  TokenType::WITH, 								
								  PqlToken(TokenType::PROCNAME, "procName"), 								  
								  PqlToken(TokenType::NONE, "")
		)};

	QueryExtractor sut(valid_with);
	PqlQuery results = sut.extractSemantics();

	REQUIRE(isSameClauses(ans, results.clauses));
}

TEST_CASE("Test Mulitple With clause")
{
	vector<Clause> ans = { Clause(PqlToken(TokenType::NONE, ""),
								  PqlToken(TokenType::SYNONYM, "p"),
								  PqlToken(TokenType::STRING, "answer"),
								  TokenType::WITH,
								  PqlToken(TokenType::PROCNAME, "procName"),
								  PqlToken(TokenType::NONE, "")
		),
							Clause(PqlToken(TokenType::NONE, ""),
									PqlToken(TokenType::STRING, "answer2"),
									PqlToken(TokenType::SYNONYM, "p"),
								  TokenType::WITH,
								  PqlToken(TokenType::NONE, ""),
								  PqlToken(TokenType::PROCNAME, "procName")
		) };

	QueryExtractor sut(valid_multi_with);
	PqlQuery results = sut.extractSemantics();

	REQUIRE(isSameClauses(ans, results.clauses));
}

TEST_CASE("Test Uses clause")
{
	vector<Clause> ans = { Clause(PqlToken(TokenType::USES, "uses"), 
								  PqlToken(TokenType::STATEMENT_NUM, "1"),
								  PqlToken(TokenType::SYNONYM, "v"),
								  TokenType::SUCH_THAT) };

	QueryExtractor sut(valid_uses);
	PqlQuery results = sut.extractSemantics();

	REQUIRE(isSameClauses(ans, results.clauses));
}

TEST_CASE("Test Modifies clause")
{
	vector<Clause> ans = { Clause(PqlToken(TokenType::MODIFIES, "modifies"),
								  PqlToken(TokenType::STATEMENT_NUM, "1"),
								  PqlToken(TokenType::SYNONYM, "v"),
								  TokenType::SUCH_THAT) };

	QueryExtractor sut(valid_modifies);
	PqlQuery results = sut.extractSemantics();

	REQUIRE(isSameClauses(ans, results.clauses));
}

TEST_CASE("Test Follows clause")
{
	vector<Clause> ans = { Clause(PqlToken(TokenType::FOLLOWS, "follows"),
								  PqlToken(TokenType::SYNONYM, "s"),
								  PqlToken(TokenType::STATEMENT_NUM, "1"),
								  TokenType::SUCH_THAT) };

	QueryExtractor sut(valid_follows);
	PqlQuery results = sut.extractSemantics();

	REQUIRE(isSameClauses(ans, results.clauses));
}

TEST_CASE("Test Follows* clause")
{
	vector<Clause> ans = { Clause(PqlToken(TokenType::FOLLOWS_A, "follows*"),
								  PqlToken(TokenType::SYNONYM, "s"),
								  PqlToken(TokenType::STATEMENT_NUM, "1"),
								  TokenType::SUCH_THAT) };

	QueryExtractor sut(valid_follows_a);
	PqlQuery results = sut.extractSemantics();

	REQUIRE(isSameClauses(ans, results.clauses));
}

TEST_CASE("Test Parent clause")
{
	vector<Clause> ans = { Clause(PqlToken(TokenType::PARENT, "parent"),
								  PqlToken(TokenType::SYNONYM, "s"),
								  PqlToken(TokenType::STATEMENT_NUM, "1"),
								  TokenType::SUCH_THAT) };

	QueryExtractor sut(valid_parent);
	PqlQuery results = sut.extractSemantics();

	REQUIRE(isSameClauses(ans, results.clauses));
}

TEST_CASE("Test Parent* clause")
{
	vector<Clause> ans = { Clause(PqlToken(TokenType::PARENT_A, "parent*"),
								  PqlToken(TokenType::SYNONYM, "s"),
								  PqlToken(TokenType::STATEMENT_NUM, "1"),
								  TokenType::SUCH_THAT) };

	QueryExtractor sut(valid_parent_a);
	PqlQuery results = sut.extractSemantics();

	REQUIRE(isSameClauses(ans, results.clauses));
}

TEST_CASE("Test Calls clause")
{
	vector<Clause> ans = { Clause(PqlToken(TokenType::CALLS, "calls"),
								  PqlToken(TokenType::SYNONYM, "v"),
								  PqlToken(TokenType::WILDCARD, "_"),
								  TokenType::SUCH_THAT) };

	QueryExtractor sut(valid_calls);
	PqlQuery results = sut.extractSemantics();

	REQUIRE(isSameClauses(ans, results.clauses));
}

TEST_CASE("Test Calls* clause")
{
	vector<Clause> ans = { Clause(PqlToken(TokenType::CALLS_A, "calls*"),
								  PqlToken(TokenType::SYNONYM, "v"),
								  PqlToken(TokenType::WILDCARD, "_"),
								  TokenType::SUCH_THAT) };

	QueryExtractor sut(valid_calls_a);
	PqlQuery results = sut.extractSemantics();

	REQUIRE(isSameClauses(ans, results.clauses));
}

TEST_CASE("Test Pattern then Such That clause")
{
	vector<Clause> ans = { Clause(PqlToken(TokenType::SYNONYM, "a"),
									PqlToken(TokenType::SYNONYM, "v"),
									PqlToken(TokenType::WILDCARD, "_"),
								  TokenType::PATTERN),
							Clause(PqlToken(TokenType::USES, "uses"),
									PqlToken(TokenType::STATEMENT_NUM, "1"),
									PqlToken(TokenType::SYNONYM, "v"),
								  TokenType::SUCH_THAT) };

	QueryExtractor sut(valid_pattern_then_such_that);
	PqlQuery results = sut.extractSemantics();

	REQUIRE(isSameClauses(ans, results.clauses));
}

TEST_CASE("Test Such That then Pattern clause")
{
	vector<Clause> ans = { Clause(PqlToken(TokenType::USES, "uses"),
									PqlToken(TokenType::STATEMENT_NUM, "1"),
									PqlToken(TokenType::SYNONYM, "v"),
									TokenType::SUCH_THAT),
							Clause(PqlToken(TokenType::SYNONYM, "a"),
									PqlToken(TokenType::SYNONYM, "v"),
									PqlToken(TokenType::WILDCARD, "_"),
								  TokenType::PATTERN)};

	QueryExtractor sut(valid_such_that_then_pattern);
	PqlQuery results = sut.extractSemantics();

	REQUIRE(isSameClauses(ans, results.clauses));
}


TEST_CASE("Test Mulitple Pattern and Such That clauses")
{
	vector<Clause> ans = { Clause(PqlToken(TokenType::SYNONYM, "a"),
								PqlToken(TokenType::SYNONYM, "v"),
								PqlToken(TokenType::WILDCARD, "_"),
							  TokenType::PATTERN),
						Clause(PqlToken(TokenType::SYNONYM, "a"),
								PqlToken(TokenType::SYNONYM, "v"),
								PqlToken(TokenType::WILDCARD, "_"),
							  TokenType::PATTERN),
						Clause(PqlToken(TokenType::USES, "uses"),
								PqlToken(TokenType::STATEMENT_NUM, "1"),
								PqlToken(TokenType::SYNONYM, "v"),
								TokenType::SUCH_THAT),
						Clause(PqlToken(TokenType::MODIFIES, "modifies"),
								PqlToken(TokenType::STATEMENT_NUM, "1"),
								PqlToken(TokenType::SYNONYM, "v"),
								TokenType::SUCH_THAT) };

	QueryExtractor sut(valid_multi_pattern_then_multi_such_that);
	PqlQuery results = sut.extractSemantics();

	REQUIRE(isSameClauses(ans, results.clauses));
}

TEST_CASE("Test Mulitple Pattern, With and Such That clauses")
{
	vector<Clause> ans = { Clause(PqlToken(TokenType::SYNONYM, "a"),
								PqlToken(TokenType::SYNONYM, "v"),
								PqlToken(TokenType::WILDCARD, "_"),
							  TokenType::PATTERN),
						Clause(PqlToken(TokenType::SYNONYM, "a"),
								PqlToken(TokenType::SYNONYM, "v"),
								PqlToken(TokenType::WILDCARD, "_"),
							  TokenType::PATTERN),
						Clause(PqlToken(TokenType::NONE, ""),
								  PqlToken(TokenType::SYNONYM, "p"),
								  PqlToken(TokenType::STRING, "one"),
								  TokenType::WITH,
								  PqlToken(TokenType::PROCNAME, "procName"),
								  PqlToken(TokenType::NONE, "")
							),
							Clause(PqlToken(TokenType::NONE, ""),
									PqlToken(TokenType::STRING, "two"),
									PqlToken(TokenType::SYNONYM, "p"),
								  TokenType::WITH,
								  PqlToken(TokenType::NONE, ""),
								  PqlToken(TokenType::PROCNAME, "procName")),
						Clause(PqlToken(TokenType::USES, "uses"),
								PqlToken(TokenType::STATEMENT_NUM, "1"),
								PqlToken(TokenType::SYNONYM, "v"),
								TokenType::SUCH_THAT),
						Clause(PqlToken(TokenType::MODIFIES, "modifies"),
								PqlToken(TokenType::STATEMENT_NUM, "1"),
								PqlToken(TokenType::SYNONYM, "v"),
								TokenType::SUCH_THAT) };

	QueryExtractor sut(valid_multi_pattern_with_such_that);
	PqlQuery results = sut.extractSemantics();

	REQUIRE(isSameClauses(ans, results.clauses));
}