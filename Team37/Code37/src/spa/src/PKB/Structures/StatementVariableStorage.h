#pragma once
#ifndef SPA_STMTVARIABLESTORE_H
#define SPA_STMTVARIABLESTORE_H

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>

using namespace std;

#include "AST/TNode.h"

typedef std::multimap<int, string>::iterator MMAPForwardIterator;
typedef std::multimap<string, int>::iterator MMAPReverseIterator;

class StatementVariableStorage {
private:
	multimap<int, string> forwardStore = {};
	multimap<string, int> reverseStore = {};

public:
	void store(int, string);

	bool retrieve(int, string);

	vector<string> forwardRetrieve(int stmt);

	vector<int> reverseRetrieve(string var);
};
#endif