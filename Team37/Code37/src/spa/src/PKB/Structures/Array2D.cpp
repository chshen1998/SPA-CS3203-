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
    matrix[index1][index2] = value;
}

bool Array2D::retrieve(int index1, int index2) {
    return matrix[index1][index2];
}

vector<int> Array2D::forwardRetrieve(int index1) {
    Row v = matrix[index1];
    vector<int> nds;
    for (int i = 0; i != v.size(); ++i) {
        if (v[i]) nds.push_back(i);
    }
    return nds;
}

vector<int> Array2D::reverseRetrieve(int index2) {
    vector<int> nds = {};
    for (int i = 0; i < size; ++i) {
        if (matrix[i][index2]) {
            nds.push_back(i);
        }
    }

    return nds;
}




