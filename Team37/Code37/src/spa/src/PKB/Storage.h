#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <set>

using namespace std;

#include "../AST/TNode.h"

class Storage {
private:
	shared_ptr<TNode> AST;
	set<string> variables;
public:
	// Constructor
	Storage();

	void storeAST(shared_ptr<TNode>);
	void storeVar(string);

	set<string> getAllVar();
};