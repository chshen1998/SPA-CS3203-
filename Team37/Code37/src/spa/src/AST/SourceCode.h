#ifndef TEAM37_SOURCECODE_H
#define TEAM37_SOURCECODE_H

#include <vector>
#include <string>

#include "Procedure.h"
#include "CFG/CFG.h"

class SourceCode : public TNode, public enable_shared_from_this<SourceCode> {
private:
    vector<shared_ptr<Procedure> > procedureLst = {};
    string filename = "";
    int numOfStatements = 1000;
public:
    SourceCode(string filename);

    /**
     * Adds a procedure to the procedure list
     *
     * @param procedure the procedure to add to the list of the source code
     * @return none
     */
    void addProcedure(shared_ptr<Procedure> procedure);

    /**
     * Gets the procedure list
     *
     * @return the list of procedures
     */
    vector<shared_ptr<Procedure> > getProcedures();

    /**
     * Gets filename of source code
     *
     * @return filename
     */
    string getFileName();

    /**
     * Sets the total number of statements
     *
     * @param num number to set
     */
    void setNumOfStatements(int num);

    /**
     * Gets the total number of statements
     *
     * @return the total number of statements
     */
    int getNumOfStatements() const;

    void accept(shared_ptr<ASTVisitor> visitor) override;

    void accept(shared_ptr<CFGVisitor> visitor);

    /**
     * Gets all the CFG associated with each procedure in the source code.
     * @return vector containing all CFGs in the program
     */
    vector<shared_ptr<CFG> > getAllCFGs();

    /**
     * Gets all the statement number to CFG node mappings for every procedure
     * @return all the statement number to CFG node mappings for every procedure
     */
    vector<shared_ptr<map<int, shared_ptr<CFGNode> > > > getAllCFGMaps();

    bool operator==(const SourceCode &other) {
        return procedureLst == other.procedureLst && filename == other.filename;
    };

    bool operator<(const SourceCode &other) {
        return filename < other.filename;
    };

};

#endif
