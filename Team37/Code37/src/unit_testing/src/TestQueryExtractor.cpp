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
	QueryExtractor sut(basic_tokens);
	PqlQuery results = sut.extractSemantics();

	REQUIRE(results.select == "v");
}

TEST_CASE("Test Pattern clause")
{
	vector<Clause> ans = { Clause(PqlToken(TokenType::SYNONYM, "a"),
								  PqlToken(TokenType::SYNONYM, "v"),
								  PqlToken(TokenType::WILDCARD, "_"))};

	QueryExtractor sut(valid_pattern);
	PqlQuery results = sut.extractSemantics();

	REQUIRE(isSameClauses(ans, results.patternClauses));
}

TEST_CASE("Test Uses clause")
{
	vector<Clause> ans = { Clause(PqlToken(TokenType::USES, "uses"), 
								  PqlToken(TokenType::STATEMENT_NUM, "1"),
								  PqlToken(TokenType::SYNONYM, "v")) };

	QueryExtractor sut(valid_uses);
	PqlQuery results = sut.extractSemantics();

	REQUIRE(isSameClauses(ans, results.suchThatClauses));
}

TEST_CASE("Test Modifies clause")
{
	vector<Clause> ans = { Clause(PqlToken(TokenType::MODIFIES, "modifies"),
								  PqlToken(TokenType::STATEMENT_NUM, "1"),
								  PqlToken(TokenType::SYNONYM, "v")) };

	QueryExtractor sut(valid_modifies);
	PqlQuery results = sut.extractSemantics();

	REQUIRE(isSameClauses(ans, results.suchThatClauses));
}

TEST_CASE("Test Follows clause")
{
	vector<Clause> ans = { Clause(PqlToken(TokenType::FOLLOWS, "follows"),
								  PqlToken(TokenType::SYNONYM, "s"),
								  PqlToken(TokenType::STATEMENT_NUM, "1")) };

	QueryExtractor sut(valid_follows);
	PqlQuery results = sut.extractSemantics();

	REQUIRE(isSameClauses(ans, results.suchThatClauses));
}

TEST_CASE("Test Follows* clause")
{
	vector<Clause> ans = { Clause(PqlToken(TokenType::FOLLOWS_A, "follows*"),
								  PqlToken(TokenType::SYNONYM, "s"),
								  PqlToken(TokenType::STATEMENT_NUM, "1")) };

	QueryExtractor sut(valid_follows_a);
	PqlQuery results = sut.extractSemantics();

	REQUIRE(isSameClauses(ans, results.suchThatClauses));
}

TEST_CASE("Test Parent clause")
{
	vector<Clause> ans = { Clause(PqlToken(TokenType::PARENT, "parent"),
								  PqlToken(TokenType::SYNONYM, "s"),
								  PqlToken(TokenType::STATEMENT_NUM, "1")) };

	QueryExtractor sut(valid_parent);
	PqlQuery results = sut.extractSemantics();

	REQUIRE(isSameClauses(ans, results.suchThatClauses));
}

TEST_CASE("Test Parent* clause")
{
	vector<Clause> ans = { Clause(PqlToken(TokenType::PARENT_A, "parent*"),
								  PqlToken(TokenType::SYNONYM, "s"),
								  PqlToken(TokenType::STATEMENT_NUM, "1")) };

	QueryExtractor sut(valid_parent_a);
	PqlQuery results = sut.extractSemantics();

	REQUIRE(isSameClauses(ans, results.suchThatClauses));
}

TEST_CASE("Test Pattern then Such That clause")
{
	vector<Clause> p_ans = { Clause(PqlToken(TokenType::SYNONYM, "a"),
									PqlToken(TokenType::SYNONYM, "v"),
									PqlToken(TokenType::WILDCARD, "_")) };

	vector<Clause> s_ans = { Clause(PqlToken(TokenType::USES, "uses"),
									PqlToken(TokenType::STATEMENT_NUM, "1"),
									PqlToken(TokenType::SYNONYM, "v")) };

	QueryExtractor sut(valid_pattern_then_such_that);
	PqlQuery results = sut.extractSemantics();

	REQUIRE(isSameClauses(p_ans, results.patternClauses));
	REQUIRE(isSameClauses(s_ans, results.suchThatClauses));
}

TEST_CASE("Test Such That then Pattern clause")
{
	vector<Clause> p_ans = { Clause(PqlToken(TokenType::SYNONYM, "a"),
									PqlToken(TokenType::SYNONYM, "v"),
									PqlToken(TokenType::WILDCARD, "_")) };

	vector<Clause> s_ans = { Clause(PqlToken(TokenType::USES, "uses"),
									PqlToken(TokenType::STATEMENT_NUM, "1"),
									PqlToken(TokenType::SYNONYM, "v")) };

	QueryExtractor sut(valid_such_that_then_pattern);
	PqlQuery results = sut.extractSemantics();

	REQUIRE(isSameClauses(p_ans, results.patternClauses));
	REQUIRE(isSameClauses(s_ans, results.suchThatClauses));
}