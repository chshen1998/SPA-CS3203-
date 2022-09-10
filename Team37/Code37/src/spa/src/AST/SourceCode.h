#ifndef TEAM37_SOURCECODE_H
#define TEAM37_SOURCECODE_H

#include <vector>
#include <string>

#include "Procedure.h"

class SourceCode : public TNode, public enable_shared_from_this<SourceCode> {
private:
    vector<shared_ptr<Procedure> > procedureLst;
    string filename;
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

    void accept(shared_ptr<ASTVisitor> visitor) override;

    bool operator == (const SourceCode& other) {
        return procedureLst == other.procedureLst && filename == other.filename;
    };

    bool operator < (const SourceCode& other) {
        return filename < other.filename;
    };
};

#endif
