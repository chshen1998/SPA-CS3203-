#ifndef SPA_CFGNODE_H
#define SPA_CFGNODE_H

#include <iostream>
#include <memory>
#include <vector>
#include <cassert>

using namespace std;

#include "AST/TNode.h"
#include "CFG/CFGVisitor/CFGVisitor.h"

class CFGNode : public enable_shared_from_this<CFGNode> {
private:
    shared_ptr<CFGNode> stmtToStore = nullptr;
    vector<shared_ptr<CFGNode>> parents = {};
    vector<shared_ptr<CFGNode>> children = {};
    shared_ptr<TNode> tNode = nullptr;
    int visited = 0;

public:
    CFGNode(shared_ptr<TNode> tNode, vector<shared_ptr<CFGNode>> parents);

    /**
     * Gets the parents of the CFGNode
     * @return parents of CFGNode
     */
    vector<shared_ptr<CFGNode>> getParents();

    /**
     * Gets all children of the CFGNode
     * @return a vector containing all child CFGNodes
     */
    vector<shared_ptr<CFGNode>> getChildren();

    /**
     * Gets a specific child using the index
     * @param i index of child
     * @return child CFGNode at index i
     */
    shared_ptr<CFGNode> getChild(int i);

    /**
     * Sets the parent of the CFGNode to newParent
     * @param newParent the new parent of the CFGNode
     */
    void addParent(shared_ptr<CFGNode> newParent);

    /**
     * Adds a child to the CFGNode
     * @param newChild the new child of the CFGNode
     */
    void addChild(shared_ptr<CFGNode> newChild);

    /**
     * Gets the total number of children nodes a CFGNode has
     * @return number of child CFGNodes
     */
    int getNumChildren();

    /**
     * Gets the associated TNode
     * @return the associated TNode
     */
    shared_ptr<TNode> getTNode();

    /**
     * Flags the CFGNode as visited
     */
    void setAsVisited();

    /**
     * Stores a while or if statement when needed as building a cfg node
     * for a while or if statement returns a dummy node
     * @param stmt while or if statement to store
     */
    void setStmtToStore(shared_ptr<CFGNode> stmt);

    /**
     * Gets the stored while or if statement
     * @return the stored while or if statement
     */
    shared_ptr<CFGNode> getStoredStmt();

    /**
     * Accepts a visitor
     *
     * @param visitor the visitor to accept
     * @return none
     */
    void accept(shared_ptr<CFGVisitor> visitor);

};


#endif //SPA_CFGNODE_H