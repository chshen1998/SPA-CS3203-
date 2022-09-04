#include "Statement/Statement.h"
#include <vector>

class Procedure : public TNode {
private:
    string procedureName;
    vector<shared_ptr<Statement>> stmtLst;
public:
    Procedure(shared_ptr<TNode> parent, string procedureName);

    /**
     * Adds a statement to the statement list
     *
     * @param stmt the statement to add to the list of the procedure
     * @return none
     */
    void addStatement(shared_ptr<Statement> stmt);

    /**
     * Get the procedure name
     *
     * @return the procedure name
     */
    string getProcedureName();

    /**
     * Gets the statement list
     *
     * @return the list of statements
     */
    vector<shared_ptr<Statement>> getStatements();

    /**
     * Gets the parent
     *
     * @return the parent
     */
    shared_ptr<TNode> getParent() override;
};
