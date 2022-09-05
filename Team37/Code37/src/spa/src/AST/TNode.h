#include <stdio.h>
#include <iostream>

using namespace std;

#ifndef TEAM37_TNODE_H
#define TEAM37_TNODE_H

class TNode {
private:
    shared_ptr<TNode> parent;
protected:
    TNode(shared_ptr<TNode> parent) : parent(parent) { }

    virtual shared_ptr<TNode> getParent() {
        return parent;
    }
};

#endif