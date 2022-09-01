#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <set>

using namespace std;

#include "PKB.h"
#include "../AST/TNode.h"

// Constructor
Storage() {}

void storeAST(shared_ptr<TNode> AST) {
	this->AST = AST;
}

void storeVar(string varName) {
	(this->variables).insert(varName);
}

set<string> getAllVar() {
	return this->variables;
 }