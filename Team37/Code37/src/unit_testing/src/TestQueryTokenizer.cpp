#include "QPS/QueryTokenizer.h"
#include "QPS/Structures/PqlError.h"
#include "QPS/Structures/PqlToken.h"
#include "QPS/Structures/PqlQuery.h"
#include "QPS/Types/ErrorType.h"
#include "QPS/Types/TokenType.h"

#include "catch.hpp"
#include <iostream>
#include <string>

using namespace std;

string inputQuery = "";
QueryTokenizer q = QueryTokenizer("");

TEST_CASE("Syntatically Valid and Correct Cases for Advanced SPA") {

    SECTION("tuples in Select statement") {
        inputQuery = R"(variable x, y, z; 
                        Select <x,y, z>)";
        q.resetQueryString(inputQuery);

        vector<string> expectedDelimited = { "variable", "x", ",", "y", ",", "z", ";","Select", "<", "x", ",", "y", ",", "z", ">"};
        vector<PqlToken> expectedTokens = {
                PqlToken(TokenType::VARIABLE, "variable"),
                PqlToken(TokenType::SYNONYM, "x"),
                PqlToken(TokenType::COMMA, ","),
                PqlToken(TokenType::SYNONYM, "y"),
                PqlToken(TokenType::COMMA, ","),
                PqlToken(TokenType::SYNONYM, "z"),
                PqlToken(TokenType::SEMICOLON, ";"),
                PqlToken(TokenType::DECLARATION_END, ""),
                PqlToken(TokenType::SELECT, "Select"),
                PqlToken(TokenType::OPEN_ARROW, "<"),
                PqlToken(TokenType::SYNONYM, "x"),
                PqlToken(TokenType::COMMA, ","),
                PqlToken(TokenType::SYNONYM, "y"),
                PqlToken(TokenType::COMMA, ","),
                PqlToken(TokenType::SYNONYM, "z"),
                PqlToken(TokenType::CLOSED_ARROW, ">"),
        };

        REQUIRE_NOTHROW(q.Tokenize());
        REQUIRE(q.delimited_query == expectedDelimited);
        REQUIRE(q.tokens == expectedTokens);
    }

    SECTION("BOOLEAN in Select statement") {
        inputQuery = R"(Select BOOLEAN such that Next* (2, 9))";
        q.resetQueryString(inputQuery);

        vector<string> expectedDelimited = { "Select", "BOOLEAN", "such", "that", "Next*", "(", "2", ",", "9", ")" };
        vector<PqlToken> expectedTokens = {
                PqlToken(TokenType::DECLARATION_END, ""),
                PqlToken(TokenType::SELECT, "Select"),
                PqlToken(TokenType::BOOLEAN, "BOOLEAN"),
                PqlToken(TokenType::SUCH, "such"),
                PqlToken(TokenType::THAT, "that"),
                PqlToken(TokenType::NEXT_A, "Next*"),
                PqlToken(TokenType::OPEN_BRACKET, "("),
                PqlToken(TokenType::STATEMENT_NUM, "2"),
                PqlToken(TokenType::COMMA, ","),
                PqlToken(TokenType::STATEMENT_NUM, "9"),
                PqlToken(TokenType::CLOSED_BRACKET, ")"),
        };

        REQUIRE_NOTHROW(q.Tokenize());
        REQUIRE(q.delimited_query == expectedDelimited);
        REQUIRE(q.tokens == expectedTokens);
    }

    SECTION("attrRef in Select statement") {
        inputQuery = R"(procedure p, q;
                        Select p.procName such that Calls(p, q))";
        q.resetQueryString(inputQuery);

        vector<string> expectedDelimited = { "procedure", "p", ",", "q", ";", "Select", "p", ".", "procName", "such", "that", "Calls", "(", "p", ",", "q", ")" };
        vector<PqlToken> expectedTokens = {
                PqlToken(TokenType::PROCEDURE, "procedure"),
                PqlToken(TokenType::SYNONYM, "p"),
                PqlToken(TokenType::COMMA, ","),
                PqlToken(TokenType::SYNONYM, "q"),
                PqlToken(TokenType::SEMICOLON, ";"),
                PqlToken(TokenType::DECLARATION_END, ""),
                PqlToken(TokenType::SELECT, "Select"),
                PqlToken(TokenType::SYNONYM, "p"),
                PqlToken(TokenType::DOT, "."),
                PqlToken(TokenType::PROCNAME, "procName"),
                PqlToken(TokenType::SUCH, "such"),
                PqlToken(TokenType::THAT, "that"),
                PqlToken(TokenType::CALLS, "Calls"),
                PqlToken(TokenType::OPEN_BRACKET, "("),
                PqlToken(TokenType::SYNONYM, "p"),
                PqlToken(TokenType::COMMA, ","),
                PqlToken(TokenType::SYNONYM, "q"),
                PqlToken(TokenType::CLOSED_BRACKET, ")"),
        };

        REQUIRE_NOTHROW(q.Tokenize());
        REQUIRE(q.delimited_query == expectedDelimited);
        REQUIRE(q.tokens == expectedTokens);
    }

    SECTION("tuples in Select statement and if pattern") {
        inputQuery = R"(if ifs; variable v; 
                        Select <ifs, v> pattern ifs(v, _, _))";
        q.resetQueryString(inputQuery);

        vector<string> expectedDelimited = { "if", "ifs", ";", "variable", "v", ";","Select", "<", "ifs", ",", "v", ">", "pattern", "ifs", "(", "v", ",", "_", ",",
                                                "_", ")" };
  
        vector<PqlToken> expectedTokens = {
                PqlToken(TokenType::IF, "if"),
                PqlToken(TokenType::SYNONYM, "ifs"),
                PqlToken(TokenType::SEMICOLON, ";"),
                PqlToken(TokenType::VARIABLE, "variable"),
                PqlToken(TokenType::SYNONYM, "v"),
                PqlToken(TokenType::SEMICOLON, ";"),
                PqlToken(TokenType::DECLARATION_END, ""),
                PqlToken(TokenType::SELECT, "Select"),
                PqlToken(TokenType::OPEN_ARROW, "<"),
                PqlToken(TokenType::SYNONYM, "ifs"),
                PqlToken(TokenType::COMMA, ","),
                PqlToken(TokenType::SYNONYM, "v"),
                PqlToken(TokenType::CLOSED_ARROW, ">"),
                PqlToken(TokenType::PATTERN, "pattern"),
                PqlToken(TokenType::SYNONYM, "ifs"),
                PqlToken(TokenType::OPEN_BRACKET, "("),
                PqlToken(TokenType::SYNONYM, "v"),
                PqlToken(TokenType::COMMA, ","),
                PqlToken(TokenType::WILDCARD, "_"),
                PqlToken(TokenType::COMMA, ","),
                PqlToken(TokenType::WILDCARD, "_"),
                PqlToken(TokenType::CLOSED_BRACKET, ")"),
        };

        REQUIRE_NOTHROW(q.Tokenize());
        REQUIRE(q.delimited_query == expectedDelimited);
        REQUIRE(q.tokens == expectedTokens);
    }

}


TEST_CASE("Syntatically Valid and Correct Cases for Basic SPA") {

    SECTION("Basic variable and Select statement") {
        inputQuery = R"(variable v; 
                        Select v)";
        q.resetQueryString(inputQuery);

        vector<string> expectedDelimited = {"variable", "v", ";", "Select", "v"};
        vector<PqlToken> expectedTokens = {
                PqlToken(TokenType::VARIABLE, "variable"),
                PqlToken(TokenType::SYNONYM, "v"),
                PqlToken(TokenType::SEMICOLON, ";"),
                PqlToken(TokenType::DECLARATION_END, ""),
                PqlToken(TokenType::SELECT, "Select"),
                PqlToken(TokenType::SYNONYM, "v"),
        };

        REQUIRE_NOTHROW(q.Tokenize());
        REQUIRE(q.delimited_query == expectedDelimited);
        REQUIRE(q.tokens == expectedTokens);
    }


    SECTION("One Such That Clause") {

        SECTION("Uses Example #1") {
            inputQuery = R"(variable v;
                         Select v such that Uses(14, v))";
            q.resetQueryString(inputQuery);

            vector<string> expectedDelimited = {"variable", "v", ";", "Select", "v", "such", "that", "Uses", "(", "14",
                                                ",", "v", ")"};
            vector<PqlToken> expectedTokens = {
                    PqlToken(TokenType::VARIABLE, "variable"),
                    PqlToken(TokenType::SYNONYM, "v"),
                    PqlToken(TokenType::SEMICOLON, ";"),
                    PqlToken(TokenType::DECLARATION_END, ""),
                    PqlToken(TokenType::SELECT, "Select"),
                    PqlToken(TokenType::SYNONYM, "v"),
                    PqlToken(TokenType::SUCH, "such"),
                    PqlToken(TokenType::THAT, "that"),
                    PqlToken(TokenType::USES, "Uses"),
                    PqlToken(TokenType::OPEN_BRACKET, "("),
                    PqlToken(TokenType::STATEMENT_NUM, "14"),
                    PqlToken(TokenType::COMMA, ","),
                    PqlToken(TokenType::SYNONYM, "v"),
                    PqlToken(TokenType::CLOSED_BRACKET, ")"),
            };

            REQUIRE_NOTHROW(q.Tokenize());
            REQUIRE(q.delimited_query == expectedDelimited);
            REQUIRE(q.tokens == expectedTokens);
        }

        SECTION("Modifies Example #1") {
            inputQuery = R"(variable v; procedure p;
                           Select p such that  Modifies(p, "x"))";
            q.resetQueryString(inputQuery);

            vector<string> expectedDelimited = {"variable", "v", ";", "procedure", "p", ";", "Select", "p", "such",
                                                "that", "Modifies", "(", "p", ",", "\"x\"", ")"};
            vector<PqlToken> expectedTokens = {
                    PqlToken(TokenType::VARIABLE, "variable"),
                    PqlToken(TokenType::SYNONYM, "v"),
                    PqlToken(TokenType::SEMICOLON, ";"),
                    PqlToken(TokenType::PROCEDURE, "procedure"),
                    PqlToken(TokenType::SYNONYM, "p"),
                    PqlToken(TokenType::SEMICOLON, ";"),
                    PqlToken(TokenType::DECLARATION_END, ""),
                    PqlToken(TokenType::SELECT, "Select"),
                    PqlToken(TokenType::SYNONYM, "p"),
                    PqlToken(TokenType::SUCH, "such"),
                    PqlToken(TokenType::THAT, "that"),
                    PqlToken(TokenType::MODIFIES, "Modifies"),
                    PqlToken(TokenType::OPEN_BRACKET, "("),
                    PqlToken(TokenType::SYNONYM, "p"),
                    PqlToken(TokenType::COMMA, ","),
                    PqlToken(TokenType::STRING, "\"x\""),
                    PqlToken(TokenType::CLOSED_BRACKET, ")"),
            };

            REQUIRE_NOTHROW(q.Tokenize());
            REQUIRE(q.delimited_query == expectedDelimited);
            REQUIRE(q.tokens == expectedTokens);
        }

        SECTION("Parent* Example #1") {
            inputQuery = R"(assign a; while w;
                Select a such that Parent*(w, a))";
            q.resetQueryString(inputQuery);

            vector<string> expectedDelimited = {"assign", "a", ";", "while", "w", ";", "Select", "a", "such", "that",
                                                "Parent*", "(", "w", ",", "a", ")"};
            vector<PqlToken> expectedTokens = {
                    PqlToken(TokenType::ASSIGN, "assign"),
                    PqlToken(TokenType::SYNONYM, "a"),
                    PqlToken(TokenType::SEMICOLON, ";"),
                    PqlToken(TokenType::WHILE, "while"),
                    PqlToken(TokenType::SYNONYM, "w"),
                    PqlToken(TokenType::SEMICOLON, ";"),
                    PqlToken(TokenType::DECLARATION_END, ""),
                    PqlToken(TokenType::SELECT, "Select"),
                    PqlToken(TokenType::SYNONYM, "a"),
                    PqlToken(TokenType::SUCH, "such"),
                    PqlToken(TokenType::THAT, "that"),
                    PqlToken(TokenType::PARENT_A, "Parent*"),
                    PqlToken(TokenType::OPEN_BRACKET, "("),
                    PqlToken(TokenType::SYNONYM, "w"),
                    PqlToken(TokenType::COMMA, ","),
                    PqlToken(TokenType::SYNONYM, "a"),
                    PqlToken(TokenType::CLOSED_BRACKET, ")"),
            };

            REQUIRE_NOTHROW(q.Tokenize());
            REQUIRE(q.delimited_query == expectedDelimited);
            REQUIRE(q.tokens == expectedTokens);
        }
    }

    SECTION("One Pattern Clause") {
        SECTION("Example #1") {
            inputQuery = R"(assign a;
                     Select a pattern a(_, "count + 1"))";
            q.resetQueryString(inputQuery);

            vector<string> expectedDelimited = {"assign", "a", ";", "Select", "a", "pattern", "a", "(", "_", ",",
                                                "\"count + 1\"", ")"};
            vector<PqlToken> expectedTokens = {
                    PqlToken(TokenType::ASSIGN, "assign"),
                    PqlToken(TokenType::SYNONYM, "a"),
                    PqlToken(TokenType::SEMICOLON, ";"),
                    PqlToken(TokenType::DECLARATION_END, ""),
                    PqlToken(TokenType::SELECT, "Select"),
                    PqlToken(TokenType::SYNONYM, "a"),
                    PqlToken(TokenType::PATTERN, "pattern"),
                    PqlToken(TokenType::SYNONYM, "a"),
                    PqlToken(TokenType::OPEN_BRACKET, "("),
                    PqlToken(TokenType::WILDCARD, "_"),
                    PqlToken(TokenType::COMMA, ","),
                    PqlToken(TokenType::STRING, "\"count + 1\""),
                    PqlToken(TokenType::CLOSED_BRACKET, ")"),
            };

            REQUIRE_NOTHROW(q.Tokenize());
            REQUIRE(q.delimited_query == expectedDelimited);
            REQUIRE(q.tokens == expectedTokens);
        }

        SECTION("One Pattern Clause Example #2") {
            inputQuery = R"(assign a; 
                        Select a pattern a ( _, _"x * y + z * t"_))";
            q.resetQueryString(inputQuery);

            vector<string> expectedDelimited = {"assign", "a", ";", "Select", "a", "pattern", "a", "(", "_", ",",
                                                "_\"x * y + z * t\"_", ")"};
            vector<PqlToken> expectedTokens = {
                    PqlToken(TokenType::ASSIGN, "assign"),
                    PqlToken(TokenType::SYNONYM, "a"),
                    PqlToken(TokenType::SEMICOLON, ";"),
                    PqlToken(TokenType::DECLARATION_END, ""),
                    PqlToken(TokenType::SELECT, "Select"),
                    PqlToken(TokenType::SYNONYM, "a"),
                    PqlToken(TokenType::PATTERN, "pattern"),
                    PqlToken(TokenType::SYNONYM, "a"),
                    PqlToken(TokenType::OPEN_BRACKET, "("),
                    PqlToken(TokenType::WILDCARD, "_"),
                    PqlToken(TokenType::COMMA, ","),
                    PqlToken(TokenType::WILDCARD_STRING, "_\"x * y + z * t\"_"),
                    PqlToken(TokenType::CLOSED_BRACKET, ")"),
            };

            REQUIRE_NOTHROW(q.Tokenize());
            REQUIRE(q.delimited_query == expectedDelimited);
            REQUIRE(q.tokens == expectedTokens);
        }

    }

    SECTION("One Such That and One Pattern Clause") {

        SECTION("Example #1") {
            inputQuery = R"(assign a; while w;
                Select a such that Parent*(w, a) pattern a ("count", _))";
            q.resetQueryString(inputQuery);

            vector<string> expectedDelimited = {"assign", "a", ";", "while", "w", ";", "Select", "a", "such", "that",
                                                "Parent*", "(", "w", ",", "a", ")", "pattern", "a", "(", "\"count\"",
                                                ",", "_", ")"};
            vector<PqlToken> expectedTokens = {
                    PqlToken(TokenType::ASSIGN, "assign"),
                    PqlToken(TokenType::SYNONYM, "a"),
                    PqlToken(TokenType::SEMICOLON, ";"),
                    PqlToken(TokenType::WHILE, "while"),
                    PqlToken(TokenType::SYNONYM, "w"),
                    PqlToken(TokenType::SEMICOLON, ";"),
                    PqlToken(TokenType::DECLARATION_END, ""),
                    PqlToken(TokenType::SELECT, "Select"),
                    PqlToken(TokenType::SYNONYM, "a"),
                    PqlToken(TokenType::SUCH, "such"),
                    PqlToken(TokenType::THAT, "that"),
                    PqlToken(TokenType::PARENT_A, "Parent*"),
                    PqlToken(TokenType::OPEN_BRACKET, "("),
                    PqlToken(TokenType::SYNONYM, "w"),
                    PqlToken(TokenType::COMMA, ","),
                    PqlToken(TokenType::SYNONYM, "a"),
                    PqlToken(TokenType::CLOSED_BRACKET, ")"),
                    PqlToken(TokenType::PATTERN, "pattern"),
                    PqlToken(TokenType::SYNONYM, "a"),
                    PqlToken(TokenType::OPEN_BRACKET, "("),
                    PqlToken(TokenType::STRING, "\"count\""),
                    PqlToken(TokenType::COMMA, ","),
                    PqlToken(TokenType::WILDCARD, "_"),
                    PqlToken(TokenType::CLOSED_BRACKET, ")"),
            };

            REQUIRE_NOTHROW(q.Tokenize());
            REQUIRE(q.delimited_query == expectedDelimited);
            REQUIRE(q.tokens == expectedTokens);
        }

        SECTION("Example #2") {
            inputQuery = R"(assign a;
                            Select a pattern a("x", _) such that Uses(a, "x"))";
            q.resetQueryString(inputQuery);

            vector<string> expectedDelimited = {"assign", "a", ";", "Select", "a", "pattern", "a", "(", "\"x\"", ",",
                                                "_", ")", "such", "that", "Uses", "(", "a", ",", "\"x\"", ")"};
            vector<PqlToken> expectedTokens = {
                    PqlToken(TokenType::ASSIGN, "assign"),
                    PqlToken(TokenType::SYNONYM, "a"),
                    PqlToken(TokenType::SEMICOLON, ";"),
                    PqlToken(TokenType::DECLARATION_END, ""),
                    PqlToken(TokenType::SELECT, "Select"),
                    PqlToken(TokenType::SYNONYM, "a"),
                    PqlToken(TokenType::PATTERN, "pattern"),
                    PqlToken(TokenType::SYNONYM, "a"),
                    PqlToken(TokenType::OPEN_BRACKET, "("),
                    PqlToken(TokenType::STRING, "\"x\""),
                    PqlToken(TokenType::COMMA, ","),
                    PqlToken(TokenType::WILDCARD, "_"),
                    PqlToken(TokenType::CLOSED_BRACKET, ")"),
                    PqlToken(TokenType::SUCH, "such"),
                    PqlToken(TokenType::THAT, "that"),
                    PqlToken(TokenType::USES, "Uses"),
                    PqlToken(TokenType::OPEN_BRACKET, "("),
                    PqlToken(TokenType::SYNONYM, "a"),
                    PqlToken(TokenType::COMMA, ","),
                    PqlToken(TokenType::STRING, "\"x\""),
                    PqlToken(TokenType::CLOSED_BRACKET, ")"),
            };

            REQUIRE_NOTHROW(q.Tokenize());
            REQUIRE(q.delimited_query == expectedDelimited);
            REQUIRE(q.tokens == expectedTokens);
        }
    }
}


TEST_CASE("Syntatically Invalid Queries") {

    SECTION("Unknown syntax in declarations") {
        inputQuery = "variable a,b,  Select. Assign assign; Select b";
        q.resetQueryString(inputQuery);

        vector<string> expectedDelimited = {"variable", "a", ",", "b", ",", "Select", ".", "Assign", "assign", ";",
                                            "Select", "b"};
        vector<PqlToken> expectedTokens = {
                PqlToken(TokenType::VARIABLE, "variable"),
                PqlToken(TokenType::SYNONYM, "a"),
                PqlToken(TokenType::COMMA, ","),
                PqlToken(TokenType::SYNONYM, "b"),
                PqlToken(TokenType::COMMA, ","),
                PqlToken(TokenType::SYNONYM, "Select"), 
                PqlToken(TokenType::UNKNOWN, "."),
                PqlToken(TokenType::SYNONYM, "Assign"),
                PqlToken(TokenType::SYNONYM, "assign"),
                PqlToken(TokenType::SEMICOLON, ";"),
                PqlToken(TokenType::DECLARATION_END, ""),
                PqlToken(TokenType::SELECT, "Select"),
                PqlToken(TokenType::SYNONYM, "b"),
        };

        REQUIRE_NOTHROW(q.Tokenize());
        REQUIRE(q.delimited_query == expectedDelimited);
        REQUIRE(q.tokens == expectedTokens);
    }

    SECTION("Multiple Such/That in such that cluase") {
        inputQuery = "call; Select pattern such such that that Uses(x, y) such that Modifies(a,b)";
        q.resetQueryString(inputQuery);

        vector<string> expectedDelimited = {"call", ";", "Select", "pattern", "such", "such", "that", "that", "Uses",
                                            "(", "x", ",", "y", ")", "such", "that", "Modifies", "(", "a", ",", "b",
                                            ")"};

        vector<PqlToken> expectedTokens = {
                PqlToken(TokenType::CALL, "call"),
                PqlToken(TokenType::SEMICOLON, ";"),
                PqlToken(TokenType::DECLARATION_END, ""),
                PqlToken(TokenType::SELECT, "Select"),
                PqlToken(TokenType::SYNONYM, "pattern"),
                PqlToken(TokenType::SUCH, "such"),
                PqlToken(TokenType::SUCH, "such"),
                PqlToken(TokenType::THAT, "that"),
                PqlToken(TokenType::THAT, "that"),
                PqlToken(TokenType::SYNONYM, "Uses"),
                PqlToken(TokenType::OPEN_BRACKET, "("),
                PqlToken(TokenType::SYNONYM, "x"),
                PqlToken(TokenType::COMMA, ","),
                PqlToken(TokenType::UNKNOWN, "y"),
                PqlToken(TokenType::CLOSED_BRACKET, ")"),
                PqlToken(TokenType::SUCH, "such"),
                PqlToken(TokenType::THAT, "that"),
                PqlToken(TokenType::MODIFIES, "Modifies"),
                PqlToken(TokenType::OPEN_BRACKET, "("),
                PqlToken(TokenType::SYNONYM, "a"),
                PqlToken(TokenType::COMMA, ","),
                PqlToken(TokenType::SYNONYM, "b"),
                PqlToken(TokenType::CLOSED_BRACKET, ")"),
        };

        REQUIRE_NOTHROW(q.Tokenize());
        REQUIRE(q.delimited_query == expectedDelimited);
        REQUIRE(q.tokens == expectedTokens);
    }
}


TEST_CASE("Basic Invalid Query Strings") {

//    SECTION("Query is of length zero") {
//        inputQuery = "";
//        q.resetQueryString(inputQuery);
//        REQUIRE_THROWS_WITH(q.Tokenize(), "Invalid Query Syntax :: Query Length is zero.");
//    }

//    SECTION("Empty String (String with blanks)") {
//        inputQuery = "  \n\t\r\v\f   ";
//        q.resetQueryString(inputQuery);
//        REQUIRE_THROWS_WITH(q.Tokenize(), "Invalid Query Syntax :: Query is blank.");
//    }

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
