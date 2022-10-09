using namespace std;

#include "EvaluatorUtils.h"
#include <algorithm>
#include <iterator>
#include <unordered_set>
#include <iostream>


vector<vector<string>> EvaluatorUtils::JoinTable(const vector<vector<string>>& v1,
    const vector<vector<string>>& v2) {

    if (v2.empty()) {
        return v1;
    }

    if (v1.empty()) {
        return v2;
    }

    unordered_map<int, int> commonColumnIndexes = EvaluatorUtils::findCommonColumnIndex(v1, v2);

    return EvaluatorUtils::JoinToIntermediateTable(v1, v2, commonColumnIndexes);
}


unordered_map<int, int> EvaluatorUtils::findCommonColumnIndex(const  vector<vector<string>>& v1,
    const vector<vector<string>>& v2) {
    unordered_map<int, int> commonColumnIndexes;
    unordered_map<string, int> v1Columns;

    for (int i = 0; i < v1[0].size(); i++) {
        v1Columns.insert(pair(v1[0][i], i));
    }

    for (int j = 0; j < v2[0].size(); j++) {
        if (v1Columns.find(v2[0][j]) != v1Columns.end()) {
            commonColumnIndexes.insert(pair(v1Columns[v2[0][j]], j));
        }
    }

    return commonColumnIndexes;
}

vector<vector<string>> EvaluatorUtils::JoinToIntermediateTable(const vector<vector<string>>& v1, const vector<vector<string>>& v2, unordered_map<int, int>& columnIndexes) 
{
    vector<vector<string>> result;
    vector<string> columns;
    unordered_set<string> commonColumn;
    unordered_set<int> tableTwoCommonIndex;

    columns.reserve(v1[0].size() + v2[0].size() - columnIndexes.size());

    for (string s : v1[0]) {
        columns.push_back(s);
        commonColumn.insert(s);
    }

    for (int i = 0; i < v2[0].size(); i++) {
        if (commonColumn.find(v2[0][i]) != commonColumn.end()) {
            tableTwoCommonIndex.insert(i);
        }
        else {
            columns.push_back(v2[0][i]);
        }
    }

    // push headers
    result.push_back(columns);

    // TODO: Currently doing nested Join O(nm)
    // Consider doing hash join O(n+m)
    // But might not matter since tables shouldn't get very big anyway
    for (int i = 1; i != v1.size(); i++) {
        for (int j = 1; j != v2.size(); j++) {
            bool allCommonIndexEqual = true;

            // Check if all the common columns are equal 
            for (const auto&[tableOneIndex, tableTwoIndex] : columnIndexes) {
               if (v1[i][tableOneIndex] != v2[j][tableTwoIndex]) {
                   allCommonIndexEqual = false;
                   break;
               }
            }

            if (allCommonIndexEqual) {
                // push a new joined row
                vector<string> temp;
                temp.reserve(result[0].size());

                for (string s1 : v1[i]) {
                    temp.push_back(s1);
                }

                for (auto k = 0; k != v2[j].size(); k++) {
                    if (tableTwoCommonIndex.find(k) == tableTwoCommonIndex.end()) {
                        temp.push_back(v2[j][k]);
                    }
                }

                result.push_back(temp);
            }
        }
    }

    return result;
}


