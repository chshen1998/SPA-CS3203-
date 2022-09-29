#ifndef SP_UTILS_H
#define SP_UTILS_H

using namespace std;

#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>

class Utils {
public:
    static const string WHITESPACE;

    /**
     * Converts a text file to a string for easier parsing.
     * @param filepath filepath of sourcecode.
     * @return text file as a string.
     */
    static string fileToString(string filename);

    /**
     * Trims whitespace on left of string
     * @param s string to be trimmed
     * @return trimmed string
     */
    static string ltrim(string s);

    /**
     * Trims whitespace on right of string
     * @param s string to be trimmed
     * @return trimmed string
     */
    static string rtrim(string s);

    /**
     * Trims whitespace on left and right of string
     * @param s string to be trimmed
     * @return trimmed string
     */
    static string trim(string s);
};

#endif //SP_UTILS_H
