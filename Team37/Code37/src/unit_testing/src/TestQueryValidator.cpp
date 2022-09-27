using namespace std;

#include <vector>
#include <unordered_map>

#include "QPS/QueryValidator.h"
#include "QPS/Structures/PqlError.h"
#include "QPS/Structures/PqlToken.h"
#include "QPS/Structures/PqlQuery.h"
#include "QPS/Types/ErrorType.h"
#include "QPS/Types/TokenType.h"
#include "TestQueryExtractorUtils.h"

#include "catch.hpp"

TEST_CASE("READ")
{
	vector<PqlToken> tokens = {
	PqlToken(TokenType::READ, "read"),
	PqlToken(TokenType::SYNONYM, "r"),
	PqlToken(TokenType::SEMICOLON, ";"),
	PqlToken(TokenType::DECLARATION_END, ""),
	PqlToken(TokenType::SELECT, "select"),
	PqlToken(TokenType::SYNONYM, "r")
	};
	QueryValidator sut = QueryValidator(tokens);
	PqlError results = sut.validateQuery();

	REQUIRE(results.errorType == ErrorType::NONE);
}


TEST_CASE("Valid declarations and select")
{
	QueryValidator sut = QueryValidator(basic_tokens);
	PqlError results = sut.validateQuery();

	REQUIRE(results.errorType == ErrorType::NONE);
}

TEST_CASE("Multiple variable declarations")
{
	QueryValidator sut = QueryValidator(multi_declarations);
	PqlError results = sut.validateQuery();

	REQUIRE(results.errorType == ErrorType::NONE);
}


TEST_CASE("Error: Missing semicolon")
{
	QueryValidator sut = QueryValidator(missing_semicolon);
	PqlError results = sut.validateQuery();

	REQUIRE(results.errorType == ErrorType::SYNTAX_ERROR);
}


TEST_CASE("Error: Missing Select Clause")
{
	QueryValidator sut = QueryValidator(missing_select);
	PqlError results = sut.validateQuery();

	REQUIRE(results.errorType == ErrorType::SEMANTIC_ERROR);
}


TEST_CASE("Error: Undeclared select parameters")
{
	QueryValidator sut = QueryValidator(undeclared_select_parameter);
	PqlError results = sut.validateQuery();

	REQUIRE(results.errorType == ErrorType::SEMANTIC_ERROR);
}

TEST_CASE("Valid Pattern clause")
{
	QueryValidator sut = QueryValidator(valid_pattern);
	PqlError results = sut.validateQuery();

	REQUIRE(results.message == "");
	REQUIRE(results.errorType == ErrorType::NONE);
}

TEST_CASE("Error: Undeclared Pattern assign")
{
	QueryValidator sut = QueryValidator(undeclared_pattern_assign);
	PqlError results = sut.validateQuery();

	REQUIRE(results.errorType == ErrorType::SEMANTIC_ERROR);
}

TEST_CASE("Error: Undeclared Pattern parameters")
{
	QueryValidator sut = QueryValidator(undeclared_pattern_parameter);
	PqlError results = sut.validateQuery();

	REQUIRE(results.errorType == ErrorType::SEMANTIC_ERROR);
}

TEST_CASE("Error: Invalid Pattern parameter type")
{
	QueryValidator sut = QueryValidator(invalid_pattern_parameter);
	PqlError results = sut.validateQuery();

	REQUIRE(results.errorType == ErrorType::SEMANTIC_ERROR);
}

TEST_CASE("Error: Missing open bracket")
{
	QueryValidator sut = QueryValidator(pattern_missing_open_bracket);
	PqlError results = sut.validateQuery();

	REQUIRE(results.errorType == ErrorType::SYNTAX_ERROR);
}

TEST_CASE("Error: Missing such that")
{
	QueryValidator sut = QueryValidator(missing_such_that);
	PqlError results = sut.validateQuery();

	REQUIRE(results.errorType == ErrorType::SYNTAX_ERROR);
}

TEST_CASE("Valid Uses clause")
{
	QueryValidator sut = QueryValidator(valid_uses);
	PqlError results = sut.validateQuery();

	REQUIRE(results.errorType == ErrorType::NONE);
}

TEST_CASE("Valid Follows clause")
{
	QueryValidator sut = QueryValidator(valid_follows);
	PqlError results = sut.validateQuery();

	REQUIRE(results.errorType == ErrorType::NONE);
}

TEST_CASE("Valid Modifies clause")
{
	QueryValidator sut = QueryValidator(valid_modifies);
	PqlError results = sut.validateQuery();

	REQUIRE(results.errorType == ErrorType::NONE);
}

TEST_CASE("Valid Parent clause")
{
	QueryValidator sut = QueryValidator(valid_parent);
	PqlError results = sut.validateQuery();

	REQUIRE(results.errorType == ErrorType::NONE);
}

TEST_CASE("Valid Follows* clause")
{
	QueryValidator sut = QueryValidator(valid_follows_a);
	PqlError results = sut.validateQuery();

	REQUIRE(results.errorType == ErrorType::NONE);
}

TEST_CASE("Valid Parent* clause")
{
	QueryValidator sut = QueryValidator(valid_parent_a);
	PqlError results = sut.validateQuery();

	REQUIRE(results.errorType == ErrorType::NONE);
}

TEST_CASE("Invalid Uses arg1 wildcard")
{
	QueryValidator sut = QueryValidator(invalid_wildcard_uses);
	PqlError results = sut.validateQuery();

	REQUIRE(results.errorType == ErrorType::SEMANTIC_ERROR);
}

TEST_CASE("Invalid Modifies arg1 wildcard")
{
	QueryValidator sut = QueryValidator(invalid_wildcard_modifies);
	PqlError results = sut.validateQuery();

	REQUIRE(results.errorType == ErrorType::SEMANTIC_ERROR);
}

TEST_CASE("Valid multi-pattern and multi-such that")
{
	QueryValidator sut = QueryValidator(valid_multi_pattern_then_multi_such_that);
	PqlError results = sut.validateQuery();

	REQUIRE(results.message == "");
	REQUIRE(results.errorType == ErrorType::NONE);
}