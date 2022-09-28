#ifndef SPA_CFG_H
#define SPA_CFG_H

using namespace std;

#include "CFGNode.h"

class CFG {
private:
    shared_ptr<CFGNode> startNode = nullptr;
    shared_ptr<CFGNode> endNode = nullptr;
public:
    CFG(shared_ptr<CFGNode> startNode);

    /**
     * Gets the start CFGNode that is associated to the first TNode
     * in the procedure
     * @return the start CFGNode of the CFG
     */
    shared_ptr<CFGNode> getStartNode();

    /**
     * Gets the end CFGNode that is associated with the last TNOde
     * in the procedure
     * @return the end CFGNode of the CFG
     */
    shared_ptr<CFGNode> getEndNode();

    /**
     * Sets the end node of the CFG
     * @param newEndNode the end node to set
     */
    void setEndNode(shared_ptr<CFGNode> newEndNode);


};


#endif //SPA_CFG_H
