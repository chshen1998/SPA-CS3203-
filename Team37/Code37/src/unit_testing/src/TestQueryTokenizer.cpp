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
        
        vector<string> expectedDelimited = { "variable", "v", ";", "Select", "v" };
        vector<PqlToken> expectedTokens = {
            PqlToken(TokenType::VARIABLE, "variable"),
            PqlToken(TokenType::SYNONYM, "v"),
            PqlToken(TokenType::SEMICOLON, ";"),
            PqlToken(TokenType::DECLARATION_END, ""),
            PqlToken(TokenType::SELECT, "Select"),
            PqlToken(TokenType::SYNONYM, "v"),
            PqlToken(TokenType::END, "")
        };
      
        REQUIRE_NOTHROW(q.Tokenize());
        REQUIRE(q.delimited_query == expectedDelimited);
        REQUIRE(q.tokens == expectedTokens);
    }


    SECTION("One Such That Clause") {
        inputQuery = "variable v;\nSelect v such that Uses(14, v)";
        q.resetQueryString(inputQuery);

        vector<string> expectedDelimited = { "variable", "v", ";", "Select", "v", "such", "that", "Uses", "(", "14", ",", "v", ")" };
        vector<PqlToken> expectedTokens = {
            PqlToken(TokenType::STATEMENT, "variable"),
            PqlToken(TokenType::SYNONYM, "v"),
            PqlToken(TokenType::SEMICOLON, ";"),
            PqlToken(TokenType::DECLARATION_END, ""),
            PqlToken(TokenType::SELECT, "Select"),
            PqlToken(TokenType::SYNONYM, "v"),
            PqlToken(TokenType::SUCH, "such"),
            PqlToken(TokenType::THAT, "that"),
            PqlToken(TokenType::FOLLOWS_A, "Uses"),
            PqlToken(TokenType::OPEN_BRACKET, "("),
            PqlToken(TokenType::STATEMENT_NUM, "14"),
            PqlToken(TokenType::COMMA, ","),
            PqlToken(TokenType::FOLLOWS_A, "v"),
            PqlToken(TokenType::CLOSED_BRACKET, ")"),
            PqlToken(TokenType::END, "")
        };

        REQUIRE_NOTHROW(q.Tokenize());
        REQUIRE(q.delimited_query == expectedDelimited);
        REQUIRE(q.tokens == expectedTokens);
    }

    /*
    SECTION("One Such That Clause") {
        inputQuery = "stmt s; \n Select s such that Follows* (6, s)";
        q.resetQueryString(inputQuery);

        vector<string> expectedDelimited = { "stmt", "s", ";", "Select", "s", "such", "that", "Follows*", "(", "6", ",", "s", ")"};
        vector<PqlToken> expectedTokens = {
            PqlToken(TokenType::STATEMENT, "stmt"),
            PqlToken(TokenType::SYNONYM, "s"),
            PqlToken(TokenType::SEMICOLON, ";"),
            PqlToken(TokenType::DECLARATION_END, ""),
            PqlToken(TokenType::SELECT, "Select"),
            PqlToken(TokenType::SYNONYM, "s"),
            PqlToken(TokenType::SUCH, "such"),
            PqlToken(TokenType::THAT, "that"),
            PqlToken(TokenType::FOLLOWS_A, "Follows*"),
            PqlToken(TokenType::OPEN_BRACKET, "such"),
            PqlToken(TokenType::THAT, "that"),
            PqlToken(TokenType::FOLLOWS_A, "Follows*"),
            PqlToken(TokenType::END, "")
        };

        REQUIRE_NOTHROW(q.Tokenize());
        REQUIRE(q.delimited_query == expectedDelimited);
        REQUIRE(q.tokens == expectedTokens);
    }
    */
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


