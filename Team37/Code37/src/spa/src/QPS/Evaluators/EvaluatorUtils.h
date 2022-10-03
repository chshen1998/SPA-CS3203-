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

    vector<vector<string>> JoinTable(const vector<vector<string>>& v1,
        const vector<vector<string>>& v2);

    vector<vector<string>> JoinToIntermediateTable(const vector<vector<string>>& v1, size_t columnIndex1, const vector<vector<string>>& v2, size_t columnIndex2);

    inline bool checkWildCardStringOrString(TokenType type) {
        return type == TokenType::STRING || type == TokenType::WILDCARD_STRING;
    }

};

#endif // !TEAM37_EVALUATOR_UTILS_H


