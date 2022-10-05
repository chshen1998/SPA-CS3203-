#ifndef SPA_CFG_H
#define SPA_CFG_H

#include <vector>
#include <cassert>

using namespace std;

#include "CFGNode.h"

class CFG {
private:
    string procName = "";
    shared_ptr<CFGNode> startNode = nullptr;
public:
    CFG(shared_ptr<CFGNode> startNode, string procName);

    /**
     * Gets the name of the procedure associated with the CFG
     * @return name of the procedure associated with the CFG
     */
    string getName();

    /**
     * Gets the start CFGNode that is associated to the first TNode
     * in the procedure
     * @return the start CFGNode of the CFG
     */
    shared_ptr<CFGNode> getStartNode();

    /**
     * Sets start node of CFG
     * @param newStartNode new start node to set
     */
    void setStartNode(shared_ptr<CFGNode> newStartNode);

};


#endif //SPA_CFG_H
