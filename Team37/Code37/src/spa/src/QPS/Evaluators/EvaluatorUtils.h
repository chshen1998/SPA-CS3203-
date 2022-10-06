using namespace std;

#ifndef TEAM37_EVALUATOR_UTILS_H
#define TEAM37_EVALUATOR_UTILS_H

#include <vector>
#include <string>
#include "QPS/Types/TokenType.h"

namespace EvaluatorUtils {
    template<typename T>
    void getLineNumInteresection(vector<T>& intersection, vector<T>& table1, vector<T>& table2) {
        sort(table1.begin(), table1.end());
        sort(table2.begin(), table2.end());

        set_intersection(table1.begin(), table1.end(),
            table2.begin(), table2.end(),
            back_inserter(intersection));
    }

    vector<vector<string>> JoinTable(const vector<vector<string>>&,
        const vector<vector<string>>&);

    unordered_map<int, int> findCommonColumnIndex(const vector<vector<string>>&,
        const vector<vector<string>>&);
    
    vector<vector<string>> JoinToIntermediateTable(const vector<vector<string>>&, const vector<vector<string>>&, unordered_map<int, int>&);

    inline bool checkWildCardStringOrString(TokenType type) {
        return type == TokenType::STRING || type == TokenType::WILDCARD_STRING;
    }

    inline void printTable(const vector<vector<string>>& table) {
        for (vector<string> v : table) {
            string result = "";
            for (string s : v) {
                result += s;
                result += "\t\t";
            }
            cout << result << endl;
        }
    }

};

#endif // !TEAM37_EVALUATOR_UTILS_H


