#include "Utils.h"
#include "Keywords.h"

const string Utils::WHITESPACE = " \n\r\t\f\v";

string Utils::fileToString(string filepath) {
    string s;
    string sTotal;

    ifstream in;
    in.open(filepath);

    if (in.is_open()) {
        while(!in.eof()) {
            getline(in, s);
            sTotal += s + "\n";
        }
        in.close();
    } else {
        cout << "Unable to open file." << endl;
    }

    return sTotal;
}

string Utils::ltrim(string s) {
    size_t start = s.find_first_not_of(Utils::WHITESPACE);
    return (start == std::string::npos) ? "" : s.substr(start);
}

string Utils::rtrim(string s) {
    size_t end = s.find_last_not_of(Utils::WHITESPACE);
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

string Utils::trim(string s) {
    return rtrim(ltrim(s));
}

vector<int> Utils::getOpenIndexes(string procedure, vector<int> openIndexes, int offset, string openBracket) {
//    string openBracket = Keywords::OPEN_EGYPTIAN;

    int idx = procedure.find(openBracket);
    if (idx != -1) {
        string remaining = procedure.substr(idx + 1, string::npos);
        idx = idx + offset;
        openIndexes.push_back(idx);
        return getOpenIndexes(remaining, openIndexes, idx + 1, openBracket);
    }
    return openIndexes;
}

vector<int> Utils::getClosedIndexes(string procedure, vector<int> closedIndexes, int offset, string closeBracket) {
//    string closeBracket = Keywords::CLOSE_EGYPTIAN;

    int idx = procedure.find(closeBracket);
    if (idx != -1) {
        string remaining = procedure.substr(idx + 1, string::npos);
        idx = idx + offset;
        closedIndexes.push_back(idx);
        return getClosedIndexes(remaining, closedIndexes, idx + 1, closeBracket);
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

bool Utils::isNegative (int i) {
    return (i < 0) ;
}

bool Utils::isPositive (int i) {
    return (i >= 0);
}

vector<int> Utils::getOpIndexes(string line) {
    string operators[] = {"+", "-", "*", "/", "%"};
    vector<int> indexes;
    for (auto o : operators) {
        int i = line.find(o);
        indexes.push_back(i);
    }
    return indexes;
}

bool Utils::isOperatedExpression(string line) {
    vector<int> indexes = getOpIndexes(line);
    bool isOpExpr = any_of(indexes.begin(), indexes.end(), isPositive);
    return isOpExpr;
}

bool Utils::isConstant(string line) {
    if (!isOperatedExpression(line)) {
        for (char c : line) {
            if (std::isdigit(c) == 0) {
                return false;
            }
        }
        return true;
    }
    return false;
}

bool Utils::isVariable(string line) {
    return !isConstant(line) && !isOperatedExpression(line);
}

string Utils::removeParentheses(string line) {
    string parentheses = "(){}";
    for (auto p : parentheses) {
        line.erase(std::remove(line.begin(), line.end(), p), line.end());
    }
    return line;
}

bool Utils::isRead(string line) {
    string keyword = Keywords::READ;
    int startIdx = line.find(keyword);
    return (startIdx != -1);
}

bool Utils::isPrint(string line) {
    string keyword = Keywords::PRINT;
    int startIdx = line.find(keyword);
    return (startIdx != -1);
}

bool Utils::isRelExpr(string line) {
    vector<string> relOperators;
    vector<int> indexes;
    relOperators.push_back(">");
    relOperators.push_back(">=");
    relOperators.push_back("<=");
    relOperators.push_back("<");
    relOperators.push_back("==");
    relOperators.push_back("!=");
    for (auto o: relOperators) {
        int idx = line.find(o);
        indexes.push_back(idx);
    }
    return std::any_of(indexes.begin(), indexes.end(), isPositive);
}

