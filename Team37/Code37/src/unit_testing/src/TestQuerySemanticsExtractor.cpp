#include "QPS/QuerySemanticsExtractor.h"
#include "QPS/QPS.h"

#include <vector>

#include "catch.hpp"
using namespace std;
void require(bool b) {
    REQUIRE(b);
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

	QuerySemanticsExtractor sut = QuerySemanticsExtractor(tokens);
	PqlQuery results = sut.ExtractSemantics();
	require(results.select == "v");
}