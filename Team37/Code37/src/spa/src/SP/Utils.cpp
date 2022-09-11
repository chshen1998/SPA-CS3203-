#include "Utils.h"
#include "Keywords.h"

const string Utils::WHITESPACE = " \n\r\t\f\v";

string Utils::ltrim(string s) {
    size_t start = s.find_first_not_of(Utils::WHITESPACE);
    return (start == std::string::npos) ? "" : s.substr(start);
}

string Utils::rtrim(string s) {
    size_t end = s.find_last_not_of(Utils::WHITESPACE);
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

string Utils::trim(string s) {
    return rtrim(Utils::ltrim(s));
}

vector<int> Utils::getOpenIndexes(string procedure, vector<int> openIndexes, int offset) {
    string openBracket = Keywords::OPEN_EGYPTIAN;

    int idx = procedure.find(openBracket);
    if (idx != -1) {
        string remaining = procedure.substr(idx + 1, string::npos);
        idx = idx + offset;
        openIndexes.push_back(idx);
        return getOpenIndexes(remaining, openIndexes, idx + 1);
    }
    return openIndexes;
}

vector<int> Utils::getClosedIndexes(string procedure, vector<int> closedIndexes, int offset) {
    string openBracket = Keywords::CLOSE_EGYPTIAN;

    int idx = procedure.find(openBracket);
    if (idx != -1) {
        string remaining = procedure.substr(idx + 1, string::npos);
        idx = idx + offset;
        closedIndexes.push_back(idx);
        return getClosedIndexes(remaining, closedIndexes, idx + 1);
    }
    return closedIndexes;
}

vector<vector<int> > Utils::getSets(vector<int> openIdx, vector<int> closedIdx) {
    int i = 0;
    vector<int> allIdx = openIdx;
    vector<vector<int> > sets;

    allIdx.insert(allIdx.end(), closedIdx.begin(), closedIdx.end());
    sort(allIdx.begin(), allIdx.end());

    while (!allIdx.empty()) {
        int curr = allIdx[i];
        int next = allIdx[i + 1];

        if (std::count(openIdx.begin(), openIdx.end(), curr) && std::count(closedIdx.begin(), closedIdx.end(), next)) {
            vector<int> pair;
            pair.push_back(curr);
            pair.push_back(next);
            sets.push_back(pair);

            if (openIdx.size()==1 && closedIdx.size()==1) {
                allIdx.clear();
            } else {
                allIdx.erase(allIdx.begin() + i + 1);
                allIdx.erase(allIdx.begin() + i);
                i = 0;
            }
        } else {
            i += 1;
        }
    }
    return sets;
}