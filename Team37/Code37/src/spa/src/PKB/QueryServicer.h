#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#include "QueryServicer.h"
#include "Storage.h"
#include "ElementType.h"


class QueryServicer {
private:
	shared_ptr<Storage> storage;
public:
	// Constructor
	QueryServicer(shared_ptr<Storage>);

	// Query methods

	// Retrieve all of an element
	// TEMP: Return all variables only
	// GOAL: Take in element type and return correct type
    set<shared_ptr<TNode>> retrieveAll(ElementType type);
};