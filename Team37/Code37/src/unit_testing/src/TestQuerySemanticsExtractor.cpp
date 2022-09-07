#include "QPS/QuerySemanticsExtractor.h"
#include "QPS/QPS.h"

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

TEST_CASE("variable v; select v") {
	vector<PqlToken> tokens = {
		PqlToken(TokenType::VARIABLE, "variable"),
		PqlToken(TokenType::SYNONYM, "v"),
		PqlToken(TokenType::SEMICOLON, ";"),
		PqlToken(TokenType::EMPTY, ""),
		PqlToken(TokenType::SELECT, "select"),
		PqlToken(TokenType::SYNONYM, "v"),
		PqlToken(TokenType::END, "")
	};

	unordered_map<string, TokenType> ans = {
		{"v", TokenType::VARIABLE}
	};


	QuerySemanticsExtractor sut = QuerySemanticsExtractor(tokens);
	PqlQuery results = sut.ExtractSemantics();
		REQUIRE(results.select == "v");
		REQUIRE(isSameMap(ans, results.declarations));
}