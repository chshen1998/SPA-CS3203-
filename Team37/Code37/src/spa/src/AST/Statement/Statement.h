using namespace std;

#ifndef TEAM37_QPS_H
#define TEAM37_QPS_H

#include "../TNode.h"

class Statement : public TNode {
private:
    int lineNum;
public:
    Statement(shared_ptr<TNode> parent, int lineNum) : TNode(parent), lineNum(lineNum) { }

    /**
     * Gets the line number of the statement
     *
     * @return the line number of the statement
     */
    virtual int getLineNum() {
        return lineNum;
    }

    shared_ptr<TNode> getParent() {
        return TNode::getParent();
    }
};

#endif