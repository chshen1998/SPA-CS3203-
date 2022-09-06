using namespace std;

#ifndef TEAM37_TNODE_H
#define TEAM37_TNODE_H

#include <stdio.h>
#include <iostream>
//#include "ASTVisitor/ASTVisitor.h"

class TNode {
private:
    shared_ptr<TNode> parent;
protected:
    // Protected since you should not be able to create a TNode object
    // Should only be accessible to SourceCode, Procedure and Statement + their children
    TNode(shared_ptr<TNode> parent) : parent(parent) { }
public:
    /**
     * Returns the parent of a TNode
     *
     * @return a TNode or a nullptr
     */
    virtual shared_ptr<TNode> getParent() {
        return parent;
    }

    /**
     * Sets the parent of a TNode
     *
     * @param newParent the new parent to set
     * @return none
     */
    void setParent(shared_ptr<TNode> newParent) {
        parent = newParent;
    }

//    /**
//     * Accepts a visitor
//     * MUST be implemented by child classes
//     *
//     * @param visitor the visitor to accept
//     * @return none
//     */
//    virtual void accept(shared_ptr<ASTVisitor> visitor)  = 0;
};

#endif