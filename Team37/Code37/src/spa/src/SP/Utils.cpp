#include "Utils.h"

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