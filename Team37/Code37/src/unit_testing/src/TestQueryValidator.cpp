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

TEST_CASE("Valid select no declarations") {
    QueryValidator sut = QueryValidator(valid_select_only);
    PqlError results = sut.validateQuery();

    REQUIRE(results.message == "");
    REQUIRE(results.errorType == ErrorType::NONE);
}

TEST_CASE("Valid Calls wildcard") {
    QueryValidator sut = QueryValidator(valid_calls_wildcards);
    PqlError results = sut.validateQuery();

    REQUIRE(results.message == "");
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

TEST_CASE("Valid Select boolean") {
    QueryValidator sut = QueryValidator(valid_select_boolean);
    PqlError results = sut.validateQuery();
    REQUIRE(results.message == "");

    REQUIRE(results.errorType == ErrorType::NONE);
}


TEST_CASE("Valid Select declared boolean synonym") {
    QueryValidator sut = QueryValidator(valid_select_declared_boolean);
    PqlError results = sut.validateQuery();
    REQUIRE(results.message == "");

    REQUIRE(results.errorType == ErrorType::NONE);
}

TEST_CASE("Valid Select declared boolean synonym attrName") {
    QueryValidator sut = QueryValidator(valid_select_declared_boolean_attrname);
    PqlError results = sut.validateQuery();
    REQUIRE(results.message == "");

    REQUIRE(results.errorType == ErrorType::NONE);
}

TEST_CASE("Valid Select attrName") {
    QueryValidator sut = QueryValidator(valid_select_attrname);
    PqlError results = sut.validateQuery();

    REQUIRE(results.message == "");
    REQUIRE(results.errorType == ErrorType::NONE);
}

TEST_CASE("Valid Select tuple") {
    QueryValidator sut = QueryValidator(valid_select_tuple);
    PqlError results = sut.validateQuery();

    REQUIRE(results.message == "");
    REQUIRE(results.errorType == ErrorType::NONE);
}

TEST_CASE("Error: Select tuple missing open arrow") {
    QueryValidator sut = QueryValidator(invalid_select_missing_arrows);
    PqlError results = sut.validateQuery();

    REQUIRE(results.errorType == ErrorType::SYNTAX_ERROR);
}

TEST_CASE("Error: Select tuple missing comma")
{
    QueryValidator sut = QueryValidator(invalid_select_missing_comma);
    PqlError results = sut.validateQuery();

    REQUIRE(results.errorType == ErrorType::SYNTAX_ERROR);
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

TEST_CASE("Valid Pattern clause assign")
{
    QueryValidator sut = QueryValidator(valid_pattern_assign);
    PqlError results = sut.validateQuery();

    REQUIRE(results.message == "");
    REQUIRE(results.errorType == ErrorType::NONE);
}

TEST_CASE("Valid Pattern clause assign with BOOLEAN as assign")
{
    QueryValidator sut = QueryValidator(valid_pattern_assign_boolean);
    PqlError results = sut.validateQuery();

    REQUIRE(results.message == "");
    REQUIRE(results.errorType == ErrorType::NONE);
}


TEST_CASE("Valid Pattern clause while")
{
    QueryValidator sut = QueryValidator(valid_pattern_while);
    PqlError results = sut.validateQuery();

    REQUIRE(results.message == "");
    REQUIRE(results.errorType == ErrorType::NONE);
}

TEST_CASE("Valid Pattern clause if")
{
    QueryValidator sut = QueryValidator(valid_pattern_if);
    PqlError results = sut.validateQuery();

    REQUIRE(results.message == "");
    REQUIRE(results.errorType == ErrorType::NONE);
}

TEST_CASE("Valid multi-pattern")
{
    QueryValidator sut = QueryValidator(valid_pattern_multi);
    PqlError results = sut.validateQuery();

    REQUIRE(results.message == "");
    REQUIRE(results.errorType == ErrorType::NONE);
}

TEST_CASE("Error: Invalid Pattern wildcard string")
{
    QueryValidator sut = QueryValidator(invalid_pattern_wildcard_string);
    PqlError results = sut.validateQuery();

    REQUIRE(results.errorType == ErrorType::SEMANTIC_ERROR);
}


TEST_CASE("Error: Invalid Pattern while parameter type")
{
    QueryValidator sut = QueryValidator(invalid_pattern_while_parameters);
    PqlError results = sut.validateQuery();

    REQUIRE(results.errorType == ErrorType::SEMANTIC_ERROR);
}

TEST_CASE("Error: Invalid Pattern if missing argument")
{
    QueryValidator sut = QueryValidator(invalid_pattern_if_parameters);
    PqlError results = sut.validateQuery();

    REQUIRE(results.errorType == ErrorType::SYNTAX_ERROR);
}

TEST_CASE("Error: Undeclared Pattern assign")
{
    QueryValidator sut = QueryValidator(undeclared_pattern_assign);
    PqlError results = sut.validateQuery();

    REQUIRE(results.errorType == ErrorType::SEMANTIC_ERROR);
}

TEST_CASE("Error: Undeclared Pattern assign parameters")
{
    QueryValidator sut = QueryValidator(undeclared_pattern_parameter);
    PqlError results = sut.validateQuery();

    REQUIRE(results.errorType == ErrorType::SEMANTIC_ERROR);
}

TEST_CASE("Error: Invalid Pattern assign parameter type")
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


TEST_CASE("Valid Follows clause with double wildcard")
{
    QueryValidator sut = QueryValidator(valid_follows_double_wildcard);
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

//TEST_CASE("Valid Calls clause")
//{
//	QueryValidator sut = QueryValidator(valid_calls);
//	PqlError results = sut.validateQuery();
//
//	REQUIRE(results.errorType == ErrorType::NONE);
//}

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

//TEST_CASE("Valid Calls* clause")
//{
//	QueryValidator sut = QueryValidator(valid_calls_a);
//	PqlError results = sut.validateQuery();
//
//	REQUIRE(results.errorType == ErrorType::NONE);
//}

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

TEST_CASE("Valid With clause")
{
    QueryValidator sut = QueryValidator(valid_with);
    PqlError results = sut.validateQuery();

    REQUIRE(results.message == "");
    REQUIRE(results.errorType == ErrorType::NONE);
}

TEST_CASE("Valid multi With clause")
{
    QueryValidator sut = QueryValidator(valid_multi_with);
    PqlError results = sut.validateQuery();

    REQUIRE(results.message == "");
    REQUIRE(results.errorType == ErrorType::NONE);
}

TEST_CASE("Valid multi Pattern, With and Such That clauses")
{
    QueryValidator sut = QueryValidator(valid_multi_pattern_with_such_that);
    PqlError results = sut.validateQuery();

    REQUIRE(results.message == "");
    REQUIRE(results.errorType == ErrorType::NONE);
}

TEST_CASE("Invalid With clause synonym to attrName type mismatch")
{
    QueryValidator sut = QueryValidator(invalid_with_attrname_mismatch);
    PqlError results = sut.validateQuery();

    REQUIRE(results.errorType == ErrorType::SEMANTIC_ERROR);
}

TEST_CASE("Invalid With clause ref type mismatch")
{
    QueryValidator sut = QueryValidator(invalid_with_ref_mismatch);
    PqlError results = sut.validateQuery();

    REQUIRE(results.errorType == ErrorType::SEMANTIC_ERROR);
}


TEST_CASE("Invalid With clause parameter type")
{
    QueryValidator sut = QueryValidator(invalid_with_parameter_type);
    PqlError results = sut.validateQuery();

    REQUIRE(results.errorType == ErrorType::SEMANTIC_ERROR);
}

TEST_CASE("Invalid With clause undeclared parameters")
{
    QueryValidator sut = QueryValidator(invalid_with_undeclared_synonym);
    PqlError results = sut.validateQuery();

    REQUIRE(results.errorType == ErrorType::SEMANTIC_ERROR);
}

TEST_CASE("Invalid With clause synonym attrname")
{
    QueryValidator sut = QueryValidator(invalid_with_synonym_attrname);
    PqlError results = sut.validateQuery();

    REQUIRE(results.errorType == ErrorType::SEMANTIC_ERROR);
}

TEST_CASE("Invalid With clause missing dot between synonym and attrName")
{
    QueryValidator sut = QueryValidator(invalid_with_missing_dot);
    PqlError results = sut.validateQuery();

    REQUIRE(results.errorType == ErrorType::SEMANTIC_ERROR);
}

TEST_CASE("Invalid With clause parameters extra tokens")
{
    QueryValidator sut = QueryValidator(invalid_with_parameter_extra_tokens);
    PqlError results = sut.validateQuery();

    REQUIRE(results.errorType == ErrorType::SEMANTIC_ERROR);
}


