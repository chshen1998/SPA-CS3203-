#pragma once

#ifndef SPA_PKB_H
#define SPA_PKB_H

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "../AST/TNode.h"
#include "./Storage.h"
#include "./QueryServicer.h"

using namespace std;


class PKB {
private:
    shared_ptr<Storage> storage = nullptr;
    shared_ptr<QueryServicer> queryServicer = nullptr;
public:
    // Constructor
    PKB();

    shared_ptr<QueryServicer> getQueryServicer();

    void buildFromAst(shared_ptr<SourceCode> AST);
};
#endif