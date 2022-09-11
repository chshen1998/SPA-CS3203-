#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <set>

using namespace std;

#include "../AST/TNode.h"
#include "../AST/SourceCode.h"
#include "../AST/Expression/RelationalFactor/NameExpression.h"
#include "../AST/Expression/RelationalFactor/ConstantExpression.h"
#include "../AST/ASTVisitor/ExtractASTVisitor.h"


class Storage : public enable_shared_from_this<Storage> {
private:
    shared_ptr<SourceCode> AST;
    set<shared_ptr<TNode>> variables;
    set<shared_ptr<TNode>> constants;
public:
    // Constructor
    Storage();

    // AST
    void storeAST(shared_ptr<SourceCode>);

    shared_ptr<SourceCode> retrieveAST();

    // Variables
    void storeVar(shared_ptr<NameExpression>);

    set<shared_ptr<TNode>> getAllVar();

    // Constants
    void storeConst(shared_ptr<ConstantExpression>);

    set<shared_ptr<TNode>> getAllConst();
};