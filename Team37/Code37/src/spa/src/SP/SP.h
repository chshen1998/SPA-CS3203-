#ifndef TEAM37_SP_H
#define TEAM37_SP_H

#include <string>
#include <vector>
#include "AST/TNode.h"

class SP {
public:
    /**
     * Opens the file found at specified filepath and parses file to create SourceNode object.
     * @param filepath The filepath to source code file
     * @return Sourcecode object
     */
    static shared_ptr<SourceCode> parse(string filepath);
};

#endif