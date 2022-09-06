#ifndef TEAM37_NAMEEXPRESSION_H
#define TEAM37_NAMEEXPRESSION_H

#include "RelationalFactor.h"
#include <string>

using namespace std;

class NameExpression : public RelationalFactor {
private:
    string varName;
public:
    NameExpression(shared_ptr<TNode> parent, string varName);

    /**
     * Get the name of the variable in the name expression
     *
     * @return a name
     */
    string getVarName();
};

#endif
