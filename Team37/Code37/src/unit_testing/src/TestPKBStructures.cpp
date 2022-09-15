#include "catch.hpp"


using namespace std;

#include "PKB/Structures/Array2D.h"

TEST_CASE("Structure - Matrix") {
    Array2D matrix = Array2D(2);

    // Check initial matrix is empty
    REQUIRE(matrix.retrieve(1, 1) == false);
    REQUIRE(matrix.retrieve(2, 1) == false);
    REQUIRE(matrix.retrieve(1, 2) == false);
    REQUIRE(matrix.retrieve(2, 2) == false);

    // Check Store and retrieve
    matrix.store(1, 1, true);
    matrix.store(2, 2, true);

    REQUIRE(matrix.retrieve(1, 1) == true);
    REQUIRE(matrix.retrieve(2, 1) == false);
    REQUIRE(matrix.retrieve(1, 2) == false);
    REQUIRE(matrix.retrieve(2, 2) == true);

    // Check forward store
    vector<int> row_0 = matrix.forwardRetrieve(1);
    vector<int> row_1 = matrix.forwardRetrieve(2);

    vector<int> cmp_0{ 1 };
    vector<int> cmp_1{ 2 };

    REQUIRE(row_0 == cmp_0);
    REQUIRE(row_1 == cmp_1);

    // Check reverse store
    vector<int> col_0 = matrix.reverseRetrieve(1);
    vector<int> col_1 = matrix.reverseRetrieve(2);
    REQUIRE(col_0 == cmp_0);
    REQUIRE(col_1 == cmp_1);

    // Test build star
    Array2D testStarMatrix = Array2D(3);
    testStarMatrix.store(1, 2, true);
    testStarMatrix.store(2, 3, true);

    vector<int> cmp_star_1{ 2, 3 };
    vector<int> cmp_star_2{ 3 };
    vector<int> cmp_star_3{};

    Array2D starMatrix = testStarMatrix.buildStar();

    REQUIRE(starMatrix.forwardRetrieve(1) == cmp_star_1);
    REQUIRE(starMatrix.forwardRetrieve(2) == cmp_star_2);
    REQUIRE(starMatrix.forwardRetrieve(3) == cmp_star_3);

    // Test loop in build star terminates
    Array2D testStarMatrixLoop = Array2D(2);
    testStarMatrixLoop.store(1, 2, true);
    testStarMatrixLoop.store(2, 1, true);
    Array2D StarMatrixLoop = testStarMatrixLoop.buildStar();

    REQUIRE(StarMatrixLoop.retrieve(1, 2));
    REQUIRE(StarMatrixLoop.retrieve(1, 1));
    REQUIRE(StarMatrixLoop.retrieve(2, 1));
    REQUIRE(StarMatrixLoop.retrieve(2, 2));
}