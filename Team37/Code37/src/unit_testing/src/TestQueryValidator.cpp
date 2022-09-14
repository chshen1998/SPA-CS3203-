using namespace std;

#include <vector>
#include <unordered_map>

#include "QPS/QueryValidator.h"
#include "QPS/QPS.h"
#include "TestQueryExtractorUtils.h"

#include "catch.hpp"

TEST_CASE("Valid declarations and select clause")
{

	QueryValidator sut = QueryValidator(basic_tokens);
	PqlError results = sut.ValidateQuery();

	REQUIRE(results.errorType == ErrorType::NONE);
}


TEST_CASE("Missing semicolon")
{

	QueryValidator sut = QueryValidator(missing_semicolon);
	PqlError results = sut.ValidateQuery();

	REQUIRE(results.errorType == ErrorType::SYNTAX_ERROR);
}


TEST_CASE("Missing Select Clause")
{

	QueryValidator sut = QueryValidator(missing_select);
	PqlError results = sut.ValidateQuery();

	REQUIRE(results.errorType == ErrorType::SEMANTIC_ERROR);
}


TEST_CASE("Undeclared select parameters")
{

	QueryValidator sut = QueryValidator(undeclared_select_parameter);
	PqlError results = sut.ValidateQuery();

	REQUIRE(results.errorType == ErrorType::SEMANTIC_ERROR);
}