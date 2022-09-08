#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <memory>

using namespace std;

#include "QueryServicer.h"
#include "Storage.h"

QueryServicer::QueryServicer(shared_ptr<Storage> storage) {
	this->storage = storage;
}

// Retrieve all of an element
// TEMP: Return all variables only
// GOAL: Take in element type and return correct type
set<shared_ptr<TNode>> QueryServicer::retrieveAll(ElementType type) {
	switch (type) {
	case VARIABLE:
		return storage->getAllVar();
		break;
	case CONSTANT:
		return storage->getAllConst();
		break;
	}
}	