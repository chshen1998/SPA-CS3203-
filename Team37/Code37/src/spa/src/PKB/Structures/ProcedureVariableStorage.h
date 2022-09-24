#pragma once
#ifndef SPA_PROCEDUREVARIABLESTORE_H
#define SPA_PROCEDUREVARIABLESTORE_H

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>

using namespace std;

#include "AST/TNode.h"

typedef std::multimap<string, string>::iterator MMAPIterator;


class ProcedureVariableStorage {
private:
	multimap<string, string> forwardStore = {};
	multimap<string, string> reverseStore = {};

public:
	void store(string, string);

	bool retrieve(string, string);

	vector<string> forwardRetrieve(string stmt);

	vector<string> reverseRetrieve(string var);
};
#endif