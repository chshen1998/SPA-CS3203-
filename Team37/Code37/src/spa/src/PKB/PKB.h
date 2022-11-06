#pragma once

#ifndef SPA_PKB_H
#define SPA_PKB_H

#include <iostream>
#include <memory>
#include <stdio.h>
#include <string>
#include <vector>

#include "../AST/TNode.h"
#include "./QueryServicer.h"
#include "./Storage.h"

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

    void buildFromCFG(shared_ptr<AllCFGs>);
};

#endif