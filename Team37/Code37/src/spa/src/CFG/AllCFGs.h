#ifndef SPA_ALLCFGS_H
#define SPA_ALLCFGS_H

#include <vector>
#include <map>

using namespace std;

#include "CFG.h"

class AllCFGs {
private:
    vector<shared_ptr<CFG> > allCFGs;
    shared_ptr<map<int, shared_ptr<CFGNode> > > combinedMap;
public:
    AllCFGs(vector<shared_ptr<CFG> > allCFGs, shared_ptr<map<int, shared_ptr<CFGNode> > > combinedMap);

    /**
     * Getter to get all CFGs of the program.
     * @return vector containing all CFGs of program
     */
    vector<shared_ptr<CFG> > getAllCFGs();

    /**
     * Getter to get the combined statement number to CFG node map
     * for the whole program.
     * @return combined statement number to CFG node map
     */
    shared_ptr<map<int, shared_ptr<CFGNode>>> getCombinedMap();

    /**
     * Checks if the statement exists in the combined map
     * @param stmtNum statement number of the node
     * @return boolean if statement exists
     */
    bool stmtExistsInMap(int stmtNum);

    /**
     * Gets the node corresponding to the statement number
     * @param stmtNum statement number of node
     * @return corresponding CFG node, and returns nullptr if statement not found
     */
    shared_ptr<CFGNode> getNode(int stmtNum);
};


#endif //SPA_ALLCFGS_H
