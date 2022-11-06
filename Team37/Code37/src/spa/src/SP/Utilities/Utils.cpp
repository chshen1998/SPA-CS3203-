#include "Utils.h"
#include "../FileNotFoundException.h"
#include "../InvalidSyntaxException.h"
#include "Keywords.h"

const string Utils::WHITESPACE = " \n\r\t\f\v";

string Utils::fileToString(string filepath)
{
    string s;
    string sTotal;

    ifstream in;
    in.open(filepath);

    if (in.is_open()) {
        while (!in.eof()) {
            getline(in, s);
            sTotal += s + "\n";
        }
        in.close();
    } else {
        throw FileNotFoundException((char*)"File not found!");
    }

    return sTotal;
}

string Utils::ltrim(string s)
{
    size_t start = s.find_first_not_of(Utils::WHITESPACE);
    return (start == std::string::npos) ? "" : s.substr(start);
}

string Utils::rtrim(string s)
{
    size_t end = s.find_last_not_of(Utils::WHITESPACE);
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

string Utils::trim(string s)
{
    return rtrim(ltrim(s));
}

bool Utils::validateName(string varName)
{
    // Variable name cannot be empty string
    if (varName.length() == 0) {
        return false;
    }

    char currChar;

    // First character must be a letter "A-Z" || "a-z"
    currChar = varName[0];
    if (!isalpha(currChar)) {
        return false;
    }

    varName = varName.erase(0, 1);

    for (char& c : varName) {
        if (!(isalpha(c) || isdigit(c))) {
            return false;
        }
    }
    return true;
}

bool Utils::validateInteger(string integer)
{
    // Variable name cannot be empty string
    if (integer.length() == 0) {
        return false;
    }

    for (char& c : integer) {
        if (!isdigit(c)) {
            return false;
        }
    }

    return true;
}

string Utils::stripOuterBrackets(string s)
{
    s = Utils::trim(s);
    if (s[0] != '(') {
        return s;
    }
    string processed;
    char currChar = s[0];
    s.erase(0, 1);
    processed.push_back(currChar);
    int bracketCount = 1;

    while (bracketCount != 0) {
        if (s.empty()) {
            return processed;
        }

        currChar = s[0];
        s.erase(0, 1);
        processed.push_back(currChar);
        if (currChar == '(') {
            bracketCount++;
        } else if (currChar == ')') {
            bracketCount--;
        }
    }

    if (!s.empty()) {
        processed.append(s);
        return processed;
    } else {
        processed.erase(0, 1);
        processed.pop_back();
        return stripOuterBrackets(processed);
    }
}
