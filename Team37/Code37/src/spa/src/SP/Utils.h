using namespace std;

#ifndef SPA_UTILS_H
#define SPA_UTILS_H

#include <string>

class Utils {
public:
    static const string WHITESPACE = " \n\r\t\f\v";

    static string ltrim(string s) {
        size_t start = s.find_first_not_of(WHITESPACE);
        return (start == std::string::npos) ? "" : s.substr(start);
    }

    static string rtrim(string s) {
        size_t end = s.find_last_not_of(WHITESPACE);
        return (end == std::string::npos) ? "" : s.substr(0, end + 1);
    }

    static string trim(string s) {
        return rtrim(ltrim(s));
    }
};
#endif //SPA_UTILS_H
