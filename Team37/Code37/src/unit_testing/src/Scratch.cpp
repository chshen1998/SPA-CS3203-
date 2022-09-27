#include <catch.hpp>
#include <string>
#include <vector>
#include <list>

#include "../../autotester/src/TestWrapper.h"
#include "../../autotester/src/TestWrapper.cpp"
#include "../../autotester/src/AbstractWrapper.h"

TEST_CASE("System") {
	TestWrapper test;
	string query = "stmt s;\nSelect s";
	test.parse("..\\..\\..\\..\\..\\..\\Tests37\\TestCases-Milestone1\\NoRelationships\\no_relationships_source.txt");
	list<string> lst = {};

	cout << "Here" << endl;
	test.evaluate(query, lst);
}