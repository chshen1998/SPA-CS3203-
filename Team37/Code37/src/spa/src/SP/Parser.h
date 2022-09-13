#ifndef SPA_PARSER_H
#define SPA_PARSER_H

using namespace std;

#include <string>
#include <vector>

class Parser {
public:
    /**
     * Converts a text file to a string for easier parsing.
     * @param filepath filepath of sourcecode.
     * @return text file as a string.
     */
    static string fileToString(string filename);

    /**
     * Extracts each procedure as a string.
     * @param srcCode source code as a string
     * @param procedures vector to store procedures as strings.
     * @return vector containing procedures as strings
     */
    static vector<string> extractProcedures(string srcCode, vector<string> procedures);


    /**
     * Extracts the names of each procedure stored in a vector.
     * @param procedures Vector storing the procedures.
     * @return names of each procedure.
     */
    static vector<string> extractProcNames(vector<string> procedures);

    /**
     * Extracts statements of a procedure as strings.
     * @param procedure
     * @return statements as strings.
     */
    static vector<string> extractStatements(string procedure);
};


#endif //SPA_PARSER_H
