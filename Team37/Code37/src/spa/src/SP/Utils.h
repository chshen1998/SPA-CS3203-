using namespace std;

#ifndef SPA_UTILS_H
#define SPA_UTILS_H

#include <string>

class Utils {
public:
    static const string WHITESPACE;

    /**
     * Trims whitespace on left of string
     * @param s string to be trimmed
     * @return trimmed string
     */
    static string ltrim(string s) {
        size_t start = s.find_first_not_of(WHITESPACE);
        return (start == std::string::npos) ? "" : s.substr(start);
    }

    /**
     * Trims whitespace on right of string
     * @param s string to be trimmed
     * @return trimmed string
     */
    static string rtrim(string s) {
        size_t end = s.find_last_not_of(WHITESPACE);
        return (end == std::string::npos) ? "" : s.substr(0, end + 1);
    }

    /**
     * Trims whitespace on left and right of string
     * @param s string to be trimmed
     * @return trimmed string
     */
    static string trim(string s) {
        return rtrim(ltrim(s));
    }
};
#endif //SPA_UTILS_H
