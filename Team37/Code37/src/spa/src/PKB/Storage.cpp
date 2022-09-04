#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <set>

using namespace std;

#include "PKB.h"
#include "../AST/TNode.h"

// Constructor
Storage::Storage() {}

void Storage::storeAST(shared_ptr<TNode> AST) {
	this->AST = AST;
}

void Storage::storeVar(string varName) {
	(this->variables).insert(varName);
}

set<string> Storage::getAllVar() {
	return this->variables;
 }