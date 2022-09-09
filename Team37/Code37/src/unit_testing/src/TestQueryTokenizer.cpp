#include "QPS/QueryTokenizer.h"

#include "catch.hpp"
#include <string>

TEST_CASE("QueryTokenizer Tokenize Test") {
    string s = "";
    QueryTokenizer q = QueryTokenizer(s);
    REQUIRE_THROWS(q.Tokenize());
}