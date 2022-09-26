#include "catch.hpp"

using namespace std;

#include "PKB/Structures/Array2D.h"
#include "PKB/Structures/StatementVariableStorage.h"
#include "PKB/Structures/ProcedureVariableStorage.h"

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

TEST_CASE("Structure - StatementVariableStorage") {
    StatementVariableStorage store = StatementVariableStorage();

    // Initally Empty
    REQUIRE(!store.retrieve(1, "x"));
    REQUIRE(store.forwardRetrieve(1).size() == 0);
    REQUIRE(store.reverseRetrieve("x").size() == 0);

    store.store(1, "x");
    store.store(2, "x");
    store.store(2, "y");
    store.store(3, "y");
    store.store(3, "z");

    REQUIRE(store.retrieve(1, "x"));
    REQUIRE(store.retrieve(2, "y"));
    REQUIRE(!store.retrieve(1, "z"));

    vector<string> cmp_1 = { "x" };
    vector<string> cmp_2 = { "x", "y" };
    vector<string> cmp_3 = { "y", "z" };

    REQUIRE(store.forwardRetrieve(1) == cmp_1);
    REQUIRE(store.forwardRetrieve(2) == cmp_2);
    REQUIRE(store.forwardRetrieve(3) == cmp_3);

    vector<int> cmp_x = { 1, 2 };
    vector<int> cmp_y = { 2, 3 };
    vector<int> cmp_z = { 3 };

    REQUIRE(store.reverseRetrieve("x") == cmp_x);
    REQUIRE(store.reverseRetrieve("y") == cmp_y);
    REQUIRE(store.reverseRetrieve("z") == cmp_z);
}

TEST_CASE("Structure - ProcedureVariableStorage") {
    ProcedureVariableStorage store = ProcedureVariableStorage();

    // Initally Empty
    REQUIRE(!store.retrieve("main", "x"));
    REQUIRE(store.forwardRetrieve("main").size() == 0);
    REQUIRE(store.reverseRetrieve("x").size() == 0);

    store.store("main", "x");
    store.store("test", "x");
    store.store("test", "y");
    store.store("sum", "y");
    store.store("sum", "z");

    REQUIRE(store.retrieve("main", "x"));
    REQUIRE(store.retrieve("test", "y"));
    REQUIRE(!store.retrieve("main", "z"));

    vector<string> cmp_1 = { "x" };
    vector<string> cmp_2 = { "x", "y" };
    vector<string> cmp_3 = { "y", "z" };

    REQUIRE(store.forwardRetrieve("main") == cmp_1);
    REQUIRE(store.forwardRetrieve("test") == cmp_2);
    REQUIRE(store.forwardRetrieve("sum") == cmp_3);

    vector<string> cmp_x = { "main", "test"};
    vector<string> cmp_y = { "test", "sum"};
    vector<string> cmp_z = { "sum" };

    REQUIRE(store.reverseRetrieve("x") == cmp_x);
    REQUIRE(store.reverseRetrieve("y") == cmp_y);
    REQUIRE(store.reverseRetrieve("z") == cmp_z);
}