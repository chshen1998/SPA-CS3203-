#include <stdio.h>
#include <iostream>

using namespace std;

class TNode {
private:
    shared_ptr<TNode> parent;
protected:
    TNode(shared_ptr<TNode> parent) : parent(parent) { }

    virtual shared_ptr<TNode> getParent() {
        return parent;
    }
};
