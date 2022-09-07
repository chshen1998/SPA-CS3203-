#pragma once

#ifndef TEAM37_STATEMENT_H
#define TEAM37_STATEMENT_H

using namespace std;

#include "AST/TNode.h"

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
     int getLineNum() {
        return lineNum;
     }
};

#endif //TEAM37_STATEMENT_H