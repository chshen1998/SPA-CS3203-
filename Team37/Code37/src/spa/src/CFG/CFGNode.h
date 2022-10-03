#ifndef SPA_CFGNODE_H
#define SPA_CFGNODE_H

#include <iostream>
#include <memory>
#include <vector>
#include <cassert>

using namespace std;

#include "AST/TNode.h"

class CFGNode : public enable_shared_from_this<CFGNode> {
private:
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
    void setParent(shared_ptr<CFGNode> newParent);

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

};


#endif //SPA_CFGNODE_H