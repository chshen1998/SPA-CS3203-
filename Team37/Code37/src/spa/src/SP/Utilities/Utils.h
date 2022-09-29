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
     * Returns a string of the contents of a text file
     * @param filepath filepath of text file.
     * @return text file as a string.
     */
    static string fileToString(string filepath);

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

    /**
     * Validates that a given string is a valid name
     * @param varName
     * @return true if string is a valid name, false otherwise
     */
    static bool validateName(string varName);

    /**
     * Validates that a given string is an integer
     * @param integer
     * @return true if string is a valid integer, false otherwise
     */
    static bool validateInteger(string integer);
};

#endif //SP_UTILS_H
