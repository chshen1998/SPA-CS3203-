#pragma once

#ifndef TEAM37_STATEMENT_H
#define TEAM37_STATEMENT_H

using namespace std;

#include "AST/TNode.h"
#include "CFG/CFG.h"

#include <memory>
#include <string>

class Statement : public TNode {
private:
    int lineNum = 0;
    static inline int lineNumCount = 1;

public:
    Statement(shared_ptr<TNode> parent)
        : TNode(parent)
        , lineNum(lineNumCount)
    {
        lineNumCount += 1;
    }

    /**
     * Gets the line number of the statement
     *
     * @return the line number of the statement
     */
    int getLineNum()
    {
        return lineNum;
    }

    /**
     * Gets the total amount of statements created so far
     *
     * @return an int
     */
    static int getLineNumCount()
    {
        return lineNumCount - 1;
    }

    /**
     * Resets the global lineNumCount back to 1
     */
    static void resetLineNumCount()
    {
        lineNumCount = 1;
    }

    /**
     * Builds on to the CFG by adding the statement to the CFG as a CFGNode
     * @param parents parent CFGNodes of the statement
     * @param cfg cfg to build on to
     * @return CFGNode for the statement
     */
    virtual shared_ptr<CFGNode> buildCFG(
        vector<shared_ptr<CFGNode>> parents, shared_ptr<CFG> cfg)
        = 0;
};
#endif // TEAM37_STATEMENT_H
