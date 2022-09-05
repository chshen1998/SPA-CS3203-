#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#include "QueryServicer.h"
#include "Storage.h"

QueryServicer::QueryServicer(shared_ptr<Storage> storage) {
	this->storage = storage;
}

// Retrieve all of an element
// TEMP: Return all variables only
// GOAL: Take in element type and return correct type
set<string> QueryServicer::retrieveAll() {
    return storage->getAllVar();
}