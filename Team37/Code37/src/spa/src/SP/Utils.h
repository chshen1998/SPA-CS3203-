using namespace std;

#ifndef SP_UTILS_H
#define SP_UTILS_H

#include <string>
#include <vector>

class Utils {
public:
    static const string WHITESPACE;

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
     * Recursively gets indexes of all open egyptian brackets in procedure string.
     * @param procedure procedure as a string
     * @param openIndexes vector to store indexes of open egyptian brackets
     * @param offset offset from the start of the string
     * @return Indexes of all open egyptian brackets in procedure.
     */
    static vector<int> getOpenIndexes(string procedure, vector<int> openIndexes, int offset);

    /**
     * Recursively gets indexes of all close egyptian brackets in procedure string.
     * @param procedure procedure as a string
     * @param openIndexes vector to store indexes of open egyptian brackets
     * @param offset offset from the start of the string
     * @return Indexes of all close egyptian brackets in procedure.
     */
    static vector<int> getClosedIndexes(string procedure, vector<int> closedIndexes, int offset);

    /**
     * Matches the open and close egyptian brackets to each other by finding the nearest close egyptian
     * bracket for each open bracket.
     * @param openIdx Indexes of all open egyptian brackets in procedure
     * @param closedIdx Indexes of all close egyptian brackets in procedure
     * @return vector containing all pairs of indexes (index of open bracket, index of close bracket)
     */
    static vector<vector<int> > getSets(vector<int> openIdx, vector<int> closedIdx);
};
#endif //SP_UTILS_H
