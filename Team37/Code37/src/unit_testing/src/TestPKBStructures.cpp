#include "catch.hpp"

using namespace std;

#include "PKB/Structures/Array2D.h"

TEST_CASE("Structure - Matrix") {
    Array2D matrix = Array2D(2);

    // Check initial matrix is empty
    REQUIRE(matrix.retrieve(0, 0) == false);
    REQUIRE(matrix.retrieve(1, 0) == false);
    REQUIRE(matrix.retrieve(0, 1) == false);
    REQUIRE(matrix.retrieve(1, 1) == false);

    // Check Store and retrieve
    matrix.store(0, 0, true);
    matrix.store(1, 1, true);

    REQUIRE(matrix.retrieve(0, 0) == true);
    REQUIRE(matrix.retrieve(1, 0) == false);
    REQUIRE(matrix.retrieve(0, 1) == false);
    REQUIRE(matrix.retrieve(1, 1) == true);

    // Check forward store
    vector<int> row_0 = matrix.forwardRetrieve(0);
    vector<int> row_1 = matrix.forwardRetrieve(1);

    vector<int> cmp_0{ 0 };
    vector<int> cmp_1{ 1 };

    REQUIRE(row_0 == cmp_0);
    REQUIRE(row_1 == cmp_1);

    // Check reverse store
    vector<int> col_0 = matrix.reverseRetrieve(0);
    vector<int> col_1 = matrix.reverseRetrieve(1);
    REQUIRE(col_0 == cmp_0);
    REQUIRE(col_1 == cmp_1);
}