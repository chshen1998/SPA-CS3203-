#ifndef SPA_CALLLOOPCHECKER_H
#define SPA_CALLLOOPCHECKER_H

using namespace std;

#include <string>
#include <vector>
#include <memory>

#include "AST/SourceCode.h"

class CallLoopChecker {
private:
    static inline map<string, vector<string>> procedureCallMap =
            map<string, vector<string>>();
public:
    /**
     * Checks for procedure call loops in a given AST
     * @param AST
     * @return true if there is a procedure call loop, false otherwise
     */
    static bool checkCallLoop(shared_ptr<SourceCode> AST);

    /**
     * Inserts the mappings for the procedures into the procedure call map
     * @param procedure
     */
    static void populateCallMap(shared_ptr<Procedure> procedure);

    /**
     * Inserts the mapping for the statements into the procedure call map
     * @param caller
     * @param statements
     */
    static void populateCallMap(string caller, vector<shared_ptr<Statement>> statements);

    /**
     * Inserts a mapping of caller to callee. Callee is appending to the list of strings in the values
     * @param caller
     * @param callee
     */
    static void addMapping(string caller, string callee);

    /**
     * Check that no 2 procedures have the same name
     * @param procedures
     */
    static void checkSameName(vector<shared_ptr<Procedure>> procedures);

    /**
     * Checks the procedureCallMap for loops
     * @param myMap the current state of the map
     * @param visited array of "nodes" that have already been visited
     * @param currNode the current node we are on
     *
     */
    static void checkCallMap(map<string, vector<string>> myMap, vector<string> visited, string currNode);


    // TODO: Delete temp function
    static void check() {
        for (auto it = procedureCallMap.begin(); it != procedureCallMap.end(); ++it) {
            cout << "NAME: " << it->first << endl;
            for (string s: it->second) {
                cout << s << endl;
            }
        }
    }
};


#endif //SPA_CALLLOOPCHECKER_H
