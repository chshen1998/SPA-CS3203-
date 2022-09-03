#include <vector>

#include "Procedure.h"

class SourceCode: public TNode {
private:
    vector<shared_ptr<Procedure>> procedureLst;
public:
    SourceCode();

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
};
