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
	storage = shared_ptr<Storage> Storage(new Storage());
	queryServicer = shared_ptr<QueryServicer> QueryServicer(new QueryServicer());
}


shared_ptr<QueryServicer> PKB::getQueryServicer() {
	return this->queryServicer;
}