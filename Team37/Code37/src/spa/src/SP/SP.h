#ifndef TEAM37_SP_H
#define TEAM37_SP_H

#include <string>
#include <vector>
#include "AST/TNode.h"

class SP {
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

    /**
     * Opens the file found at specified filepath and parses file to create SourceNode object.
     * @param filepath The filepath to source code file
     * @return Sourcecode object
     */
    static shared_ptr<SourceCode> parse(string filename);
};

#endif