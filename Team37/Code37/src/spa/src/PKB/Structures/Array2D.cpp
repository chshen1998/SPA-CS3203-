#include "Array2D.h"

Array2D::Array2D(int in_size) {
	size = in_size;

	// Fill with empty values
    for (size_t i = 0; i < size; ++i) {
        Row row(size);

        for (size_t j = 0; j < size; ++j) {
            row[j] = false;
        }

        matrix.push_back(row); // push each row after you fill it
    }
}

void Array2D::store(int index1, int index2, bool value) {
	if (index1 > size || index2 > size) {
		throw invalid_argument("Index out of range");
	}

	// Convert statemnt number to index
	index1--; index2--;
	
    matrix[index1][index2] = value;
}

bool Array2D::retrieve(int index1, int index2) {
	if (index1 > size || index2 > size) {
		throw invalid_argument("Index out of range");
	}

	index1--; index2--;

    return matrix[index1][index2];
}

vector<int> Array2D::forwardRetrieve(int index1) {
	if (index1 > size) {
		throw invalid_argument("Index out of range");
	}

	index1--;

    Row v = matrix[index1];
    vector<int> nds;
    for (int i = 0; i != v.size(); ++i) {
        if (v[i]) nds.push_back(i + 1);
    }
    return nds;
}

vector<int> Array2D::reverseRetrieve(int index2) {
	if (index2 > size) {
		throw invalid_argument("Index out of range");
	}

	index2--;

    vector<int> nds = {};
    for (int i = 0; i < size; ++i) {
        if (matrix[i][index2]) {
            nds.push_back(i + 1);
        }
    }

    return nds;
}


/*
* Once base statement-statement table has been filled, call this to build
* @param type Type of relation to build
*/
Array2D Array2D::buildStar() {
	Array2D starMatrix = Array2D(size);
	// Through all rows
	for (int i = 1; i <= size; ++i) {
		unordered_map<int, bool> memo;

		unordered_set<int> workingSet;
		vector<int> initalRelation = forwardRetrieve(i);
		for (int x : initalRelation) {
			workingSet.insert(x);
		}
		
		while (!workingSet.empty()) {	
			// Pop 1 item
			int currVal = *workingSet.begin();
			workingSet.erase(workingSet.begin());
			starMatrix.store(i, currVal, true);
			// Add all that forwardRetreive
			vector<int> nextVals = forwardRetrieve(currVal);

			for (int x : nextVals) {
				// Check if done before
				if (memo[x]) {
					continue;
				}
				workingSet.insert(x);				
				memo[x] = true;
			}
		}
	}

	return starMatrix;
}




