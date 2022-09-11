#include "QueryServicer.h"

QueryServicer::QueryServicer(shared_ptr<Storage> storage) {
	this->storage = storage;
}

// Retrieve all of an element
// TEMP: Return all variables only
// GOAL: Take in element type and return correct type
set<NameExpression> QueryServicer::getAllVar() {
	return storage->getAllVar();
}	

set<ConstantExpression> QueryServicer::getAllConst() {
	return storage->getAllConst();
}