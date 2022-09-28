#ifndef SPA_CFG_H
#define SPA_CFG_H

#include <vector>
#include <cassert>

using namespace std;

#include "CFGNode.h"

class CFG {
private:
    shared_ptr<CFGNode> startNode = nullptr;
    vector<shared_ptr<CFGNode>> endNodes = {};
public:
    CFG(shared_ptr<CFGNode> startNode);

    /**
     * Gets the start CFGNode that is associated to the first TNode
     * in the procedure
     * @return the start CFGNode of the CFG
     */
    shared_ptr<CFGNode> getStartNode();

    /**
     * Gets all the end CFGNodes in the CFG,
     * as there could be more than 1 end node e.g. in if-else
     * @return the end CFGNode of the CFG
     */
    vector<shared_ptr<CFGNode>> getEndNodes();

    /**
     * Gets a specific end node by the index
     * @param i index of the end node
     * @return the end CFGNode
     */
    shared_ptr<CFGNode> getEndNode(int i);

    /**
     * Sets the end node of the CFG
     * @param newEndNode the end node to set
     */
    void setEndNode(shared_ptr<CFGNode> newEndNode);


};


#endif //SPA_CFG_H
