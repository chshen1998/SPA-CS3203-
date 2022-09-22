#pragma once

#ifndef SPA_ARRAY_2D_H
#define SPA_ARRAY_2D_H

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>

using namespace std;

typedef vector< vector<int> > Matrix;
typedef vector<int> Row;

class Array2D{
private:
	int size = 0;
	Matrix matrix = {};
public:
	// Constructor
	Array2D(int);

	// Store
	void store(int index1, int index2, bool value);

	// Retrieve 1 spot
	bool retrieve(int index1, int index2);

	// Forward retrieve
	vector<int> forwardRetrieve(int index1);

	// Reverse
	vector<int> reverseRetrieve(int index2);

	// Star Build
	Array2D buildStar();
};
#endif