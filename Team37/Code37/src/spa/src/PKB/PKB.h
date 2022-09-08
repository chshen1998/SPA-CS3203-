#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>

#include "../AST/TNode.h"
#include "./Storage.h"
#include "./QueryServicer.h"

using namespace std;


class PKB {
private:
    shared_ptr<Storage> storage;
    shared_ptr<QueryServicer> queryServicer;
public:
    // Constructor
    PKB();

    shared_ptr<QueryServicer> getQueryServicer();

    void buildFromAst(shared_ptr<TNode> AST);
};