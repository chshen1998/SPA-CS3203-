#include "QPS/QueryExtractor.h"
#include "QPS/QPS.h"
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

bool isSameClause(Clause ans, Clause result)
{
	if (ans.left != result.left)
	{
		return false;
	} else if (ans.right != result.right)
	{
		return false;
	} else
	{
		return true;
	}
}


TEST_CASE("Test declartions") {
	unordered_map<string, TokenType> ans = {
		{"v", TokenType::VARIABLE},
		{"a", TokenType::ASSIGN}
	};

	QueryExtractor sut(basic_tokens);
	PqlQuery results = sut.ExtractSemantics();

	REQUIRE(isSameMap(ans, results.declarations));
}

TEST_CASE("Test Select clauses") {
	QueryExtractor sut(basic_tokens);
	PqlQuery results = sut.ExtractSemantics();

	REQUIRE(results.select == "v");
}

/*
TEST_CASE("Test Pattern clause")
{
	Clause ans;
	ans.left = "x";
	ans.right = "y";

	vector<PqlToken> tokens = addPatternClause(basic_tokens);

	QueryExtractor sut = QueryExtractor(tokens);
	PqlQuery results = sut.ExtractSemantics();

	REQUIRE(isSameClause(ans, results.patternClause));
}

TEST_CASE("Test Uses clause")
{
	Clause ans;
	ans.left = "s";
	ans.right = "v";

	vector<PqlToken> tokens = addUsesClause(basic_tokens);

	QueryExtractor sut = QueryExtractor(tokens);
	PqlQuery results = sut.ExtractSemantics();

	REQUIRE(isSameClause(ans, results.suchThatClause));
}

TEST_CASE("Test Modifies Clause")
{
	Clause ans;
	ans.left = "s";
	ans.right = "v";

	vector<PqlToken> tokens = addModifiesClause(basic_tokens);

	QueryExtractor sut = QueryExtractor(tokens);
	PqlQuery results = sut.ExtractSemantics();

	REQUIRE(isSameClause(ans, results.suchThatClause));
}

TEST_CASE("Test Parent Clause")
{
	Clause ans;
	ans.left = "s1";
	ans.right = "s2";

	vector<PqlToken> tokens = addParentClause(basic_tokens);

	QueryExtractor sut = QueryExtractor(tokens);
	PqlQuery results = sut.ExtractSemantics();

	REQUIRE(isSameClause(ans, results.suchThatClause));
}

TEST_CASE("Test Follows Clause")
{
	Clause ans;
	ans.left = "s1";
	ans.right = "s2";

	vector<PqlToken> tokens = addFollowsClause(basic_tokens);

	QueryExtractor sut = QueryExtractor(tokens);
	PqlQuery results = sut.ExtractSemantics();

	REQUIRE(isSameClause(ans, results.suchThatClause));
}

TEST_CASE("Test Pattern then Such That Clause")
{
	Clause pattern;
	pattern.left = "x";
	pattern.right = "y";
	Clause follows;
	follows.left = "s1";
	follows.right = "s2";

	vector<PqlToken> tokens = addPatternClause(basic_tokens);
	tokens = addFollowsClause(tokens);

	QueryExtractor sut = QueryExtractor(tokens);
	PqlQuery results = sut.ExtractSemantics();

	REQUIRE(isSameClause(pattern, results.patternClause));
	REQUIRE(isSameClause(follows, results.suchThatClause));
}

TEST_CASE("Test Such That then Pattern Clause")
{
	Clause pattern;
	pattern.left = "x";
	pattern.right = "y";
	Clause follows;
	follows.left = "s1";
	follows.right = "s2";

	vector<PqlToken> tokens = addFollowsClause(basic_tokens);
	tokens = addPatternClause(tokens);

	QueryExtractor sut = QueryExtractor(tokens);
	PqlQuery results = sut.ExtractSemantics();

	REQUIRE(isSameClause(pattern, results.patternClause));
	REQUIRE(isSameClause(follows, results.suchThatClause));
}

*/