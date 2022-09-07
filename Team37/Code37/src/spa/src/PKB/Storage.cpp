#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <set>

using namespace std;

#include "PKB.h"

// Constructor
Storage::Storage() {}

void Storage::storeAST(shared_ptr<TNode> AST) {
    this->AST = AST;

    vector<shared_ptr<TNode>> TNodeVariables;
    shared_ptr<ASTVisitor> visitor = make_shared<ASTVisitor>(TNodeVariables);

    AST->accept(visitor);

    //  Storing variables
    for (auto variable: visitor->getTNodeVariables()) {
        this->storeVar(variable);
    }

}

void Storage::storeVar(string varName) {
    (this->variables).insert(varName);
}

set<string> Storage::getAllVar() {
    return this->variables;
}