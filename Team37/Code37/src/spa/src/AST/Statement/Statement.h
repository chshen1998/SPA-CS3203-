#pragma once

#ifndef TEAM37_STATEMENT_H
#define TEAM37_STATEMENT_H

using namespace std;

#include "AST/TNode.h"

#include <string>
#include <memory>

class Statement : public TNode {
private:
    int lineNum = 0;
    static inline int lineNumCount = 1;
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
     static int getLineNumCount() {
         return lineNumCount - 1;
     }

     /**
      * Resets the global lineNumCount back to 1
      */
     static void resetLineNumCount() {
         lineNumCount = 1;
     }
};
#endif //TEAM37_STATEMENT_H
