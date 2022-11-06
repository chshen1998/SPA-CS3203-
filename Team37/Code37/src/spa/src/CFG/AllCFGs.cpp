#include "AllCFGs.h"

AllCFGs::AllCFGs(vector<shared_ptr<CFG>> allCFGs, shared_ptr<map<int, shared_ptr<CFGNode>>> combinedMap)
    : allCFGs(allCFGs)
    , combinedMap(combinedMap)
{
}

vector<shared_ptr<CFG>> AllCFGs::getAllCFGs()
{
    return this->allCFGs;
}

shared_ptr<map<int, shared_ptr<CFGNode>>> AllCFGs::getCombinedMap()
{
    return this->combinedMap;
}

bool AllCFGs::stmtExistsInMap(int stmtNum)
{
    if (combinedMap->find(stmtNum) == combinedMap->end()) {
        return false;
    } else {
        return true;
    }
}
shared_ptr<CFGNode> AllCFGs::getNode(int stmtNum)
{
    if (stmtExistsInMap(stmtNum)) {
        return combinedMap->at(stmtNum);
    } else {
        return nullptr;
    }
}
