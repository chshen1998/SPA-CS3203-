using namespace std;

#ifndef TEAM37_SOURCECODE_H
#define TEAM37_SOURCECODE_H

#include <vector>
#include <string>

#include "Procedure.h"
#include "ASTVisitor/ASTVisitor.h"
#include "AST/TNode.h"

class SourceCode: public TNode {
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

//    void accept(shared_ptr<ASTVisitor> visitor) override;
};

#endif
