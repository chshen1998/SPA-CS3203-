using namespace std;

#include "EvaluatorUtils.h"
#include <algorithm>
#include <iterator>


vector<vector<string>> EvaluatorUtils::JoinTable(const vector<vector<string>>& v1,
    const vector<vector<string>>& v2) {

    if (v2.empty()) {
        return v1;
    }

    if (v1.empty()) {
        return v2;
    }

    int columnIndex = -1;
    int columnIndex2 = -1;

    for (int i = 0; i < v1[0].size(); i++) {
        for (int j = 0; j < v2[0].size(); j++) {
            if (v1[0][i] == v2[0][j]) {
                columnIndex = i;
                columnIndex2 = j;
                break;
            }
        }
    }

    return EvaluatorUtils::JoinToIntermediateTable(v1, columnIndex, v2, columnIndex2);
}


vector<vector<string>> EvaluatorUtils::JoinToIntermediateTable(const vector<vector<string>>& v1, size_t columnIndex1,
    const vector<vector<string>>& v2, size_t columnIndex2) {

    vector<vector<string>> result;
    vector<string> columns;

    string commonColumn = v1[0][columnIndex1];

    for (string s : v1[0]) {
        columns.push_back(s);
    }
    for (string s : v2[0]) {
        if (s == commonColumn) continue;
        columns.push_back(s);
    }

    // push headers
    result.push_back(columns);

    for (int i = 1; i != v1.size(); i++) {

        string joiner = v1[i][columnIndex1];

        for (int j = 1; j != v2.size(); j++) {
            if (v2[j][columnIndex2] == joiner) {

                // push a new joined row
                vector<string> temp;

                for (string s1 : v1[i]) {
                    temp.push_back(s1);
                }
                for (auto k = 0; k != v2[j].size(); k++) {
                    if (k == columnIndex2) continue;
                    temp.push_back(v2[j][k]);
                }

                result.push_back(temp);
            }
        }
    }

    return result;
}


