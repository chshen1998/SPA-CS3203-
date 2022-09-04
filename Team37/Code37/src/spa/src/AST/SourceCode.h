#include <vector>
#include <string>

#include "Procedure.h"
#include "SP/Visitor.h"

class SourceCode: public TNode {
private:
    vector<shared_ptr<Procedure>> procedureLst;
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
    vector<shared_ptr<Procedure>> getProcedures();

    /**
     * Gets the parent
     *
     * @return the parent
     */
    shared_ptr<TNode> getParent() override;

    /**
     * Accepts the visitor.
     *
     * @param visitor
     */
    void accept(shared_ptr<Visitor> visitor);

    /**
     * Gets filename of source code
     *
     * @return filename
     */
    string getFileName() {
        return this->filename;
    }
};
