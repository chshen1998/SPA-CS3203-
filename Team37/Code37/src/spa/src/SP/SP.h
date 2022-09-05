using namespace std;

#ifndef TEAM37_SP_H
#define TEAM37_SP_H

#include <string>
#include <vector>
#include <AST/TNode.h>

class SP {
public:
    static shared_ptr<TNode> parse(string filename);
};

#endif