#ifndef SPA_CFG_H
#define SPA_CFG_H

#include <vector>
#include <cassert>
#include <map>

using namespace std;

#include "CFGNode.h"

class CFG {
private:
    string procName = "";
    shared_ptr<CFGNode> startNode = nullptr;
    shared_ptr<map<int, shared_ptr<CFGNode> > > stmtCFGNodeMap =
            make_shared<map<int, shared_ptr<CFGNode> > >();
public:
    CFG(shared_ptr<CFGNode> startNode, string procName);

    /**
     * Gets the name of the procedure associated with the CFG
     * @return name of the procedure associated with the CFG
     */
    string getName();

    /**
     * Set procedure name of the cfg to name
     * @param name name to set
     */
    void setName(string name);

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

    /**
     * Adds mapping of statement number to corresponding cfg node in cfg
     * @param stmtNum of the statement
     * @param cfgNode corresponding cfg node
     */
    void addMapping(int stmtNum, shared_ptr<CFGNode> cfgNode);

    /**
     * Gets the corresponding cfg node from the statement number from the
     * statement to cfg node map
     * @param stmtNum statement number
     * @return corresponding cfg node
     */
    shared_ptr<CFGNode> getCFGNode(int stmtNum);

    /**
     * Gets the stmtNumCFGNode map
     * @return the stmtNumCFGNode map
     */
    shared_ptr<map<int, shared_ptr<CFGNode> > > getMap();

};


#endif //SPA_CFG_H
