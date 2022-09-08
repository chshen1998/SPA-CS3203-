#include "QPS/QueryTokenizer.h"

#include "catch.hpp"
#include <iostream>
#include <string>

using namespace std;

string inputQuery = "";
QueryTokenizer q = QueryTokenizer("");


TEST_CASE("Valid Cases") {

    SECTION("Basic variable and Select statement") {
        inputQuery = "variable v; Select v";
        q.resetQueryString(inputQuery);
        
        vector<string> expectedDelimited = { "variable", "v", ";", "Select", "v"};
        vector<PqlToken> expectedTokens = {
            PqlToken(TokenType::VARIABLE, "variable"),
            PqlToken(TokenType::SYNONYM, "v"),
            PqlToken(TokenType::SEMICOLON, ";"),
            PqlToken(TokenType::SELECT, "Select"),
            PqlToken(TokenType::SYNONYM, "v")
        };
      
        REQUIRE_NOTHROW(q.Tokenize());
        REQUIRE(q.delimited_query == expectedDelimited);
        REQUIRE(q.tokens == expectedTokens);
    }
}


TEST_CASE("Basic Invalid Query Strings") {

    SECTION("Query is of length zero") {
        inputQuery = "";
        q.resetQueryString(inputQuery);
        REQUIRE_THROWS_WITH(q.Tokenize(), "Invalid Query Syntax :: Query Length is zero.");
    }

    SECTION("Empty String (String with blanks)") {
        inputQuery = "  \n\t\r\v\f   ";
        q.resetQueryString(inputQuery);
        REQUIRE_THROWS_WITH(q.Tokenize(), "Invalid Query Syntax :: Query is blank.");
    }

    SECTION("No `select` keyword") {
        inputQuery = "variable v;";
        q.resetQueryString(inputQuery);
        REQUIRE_THROWS(q.Tokenize(), "Invalid Query Syntax :: Select statement does not exist.");
    }

    SECTION("SemiColon at the end of query string") {
        inputQuery = "variable v; Select v;";
        q.resetQueryString(inputQuery);
        REQUIRE_THROWS(q.Tokenize(), "Invalid Query Syntax :: Query String should not end with a semicolon.");
    }

    SECTION("Invalid Token") {
        inputQuery = "!!!??%$^";
        q.resetQueryString(inputQuery);
        REQUIRE_THROWS(q.Tokenize(), "Invalid Query syntax :: Token does not exist.");
    }
}


