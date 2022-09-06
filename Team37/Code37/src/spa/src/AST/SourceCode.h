using namespace std;

#ifndef TEAM37_SOURCECODE_H
#define TEAM37_SOURCECODE_H

#include <vector>
#include <string>
#include <memory>

#include "Procedure.h"
#include "SP/Visitor.h"
#include "AST/TNode.h"

class SourceCode: public std::enable_shared_from_this<SourceCode>, public TNode {
private:
    vector<shared_ptr<Procedure> > procedureLst;
    string program;
public:
    SourceCode(string program);

    std::shared_ptr<SourceCode> getSourceCode() {
        return shared_from_this();
    }

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
     * Gets the parent
     *
     * @return the parent
     */
    shared_ptr<TNode> getParent();

    /**
     * Accepts the visitor.
     *
     * @param visitor
     */
    shared_ptr<TNode> accept(shared_ptr<Visitor> visitor);

    /**
     * Gets source code as string
     *
     * @return program
     */
    string getProgram() {
        return this->program;
    }
};
#endif //TEAM37_QPS_H