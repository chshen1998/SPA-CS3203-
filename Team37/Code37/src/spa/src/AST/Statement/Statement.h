#pragma once

#ifndef TEAM37_STATEMENT_H
#define TEAM37_STATEMENT_H

using namespace std;

#include "AST/TNode.h"

static int lineNumCount = 1;

class Statement : public TNode {
private:
    int lineNum;
public:
    Statement(shared_ptr<TNode> parent) : TNode(parent), lineNum(lineNumCount) {
        lineNumCount += 1;
    }

    /**
     * Gets the line number of the statement
     *
     * @return the line number of the statement
     */
     int getLineNum() {
        return lineNum;
     }

     /**
      * Gets the total amount of statements created so far
      *
      * @return an int
      */
     int getLineNumCount() {
         return lineNumCount - 1;
     }

     /**
      * Resets the global lineNumCount back to 1
      */
     void resetLineNumCount() {
         lineNumCount = 1;
     }
};

#endif //TEAM37_STATEMENT_H
