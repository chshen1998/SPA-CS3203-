#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#include "PKB.h"
#include "../AST/TNode.h"
#include "./Storage.h"
#include "./QueryServicer.h"

PKB::PKB() {
	storage = make_shared<Storage>();
	queryServicer = make_shared<QueryServicer>();
}


shared_ptr<QueryServicer> PKB::getQueryServicer() {
	return this->queryServicer;
}

set<string> retrieveAll() {
	return storage->getAllVar();
}