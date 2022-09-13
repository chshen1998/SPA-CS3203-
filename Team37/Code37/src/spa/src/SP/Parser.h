#ifndef SPA_PARSER_H
#define SPA_PARSER_H

using namespace std;

#include <string>
#include <vector>

class Parser {
public:
    /**
     * Extracts each procedure as a string. Ultimately produces ProcedureList.
     * @param srcCode source code as a string
     * @param procedures vector to store procedures as strings.
     * @return ProcedureList which is a vector containing procedures as strings
     */
    static vector<string> extractProcedures(string srcCode, vector<string> procedures);


    /**
     * Extracts the names of each procedure as strings to be stored in a vector.
     * @param procedures Vector storing the procedures.
     * @return names of each procedure.
     */
    static vector<string> extractProcNames(vector<string> procedures);

    /**
     * Extracts statements of a procedure as strings.
     * @param procedure
     * @return StatementList which is a vector containing statements as strings.
     */
    static vector<string> extractStatements(string procedure);
};


#endif //SPA_PARSER_H
