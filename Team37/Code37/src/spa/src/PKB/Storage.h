#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <unordered_set>

using namespace std;

#include "../AST/TNode.h"
#include "../AST/SourceCode.h"
#include "../AST/Expression/RelationalFactor/NameExpression.h"
#include "../AST/Expression/RelationalFactor/ConstantExpression.h"
#include "../AST/ASTVisitor/ExtractGeneralASTVisitor.h"
#include "../AST/ASTVisitor/ExtractFollowsASTVisitor.h"
#include "../AST/ASTVisitor/ExtractParentsASTVisitor.h"

#include "../AST/Statement/Statement.h"
#include "Structures/Array2D.h"
#include "Structures/StatementVariableStorage.h"
#include "Types/StmtStmtRelationType.h"
#include "Types/StmtVarRelationType.h"


class Storage : public enable_shared_from_this<Storage> {
private:
    shared_ptr<SourceCode> AST;
    set<NameExpression> variables;
    set<ConstantExpression> constants;
    set<shared_ptr<Statement>> statements;

    Array2D Follows = NULL;
    Array2D FollowsS = NULL;
    Array2D Parent = NULL;
    Array2D ParentS = NULL;

    StatementVariableStorage UsesV;
    StatementVariableStorage ModifiesV;
public:
    // Constructor
    Storage();

    // AST
    void storeAST(shared_ptr<SourceCode>);

    shared_ptr<SourceCode> retrieveAST();

    // Variables
    void storeVar(NameExpression);

    set<NameExpression> getAllVar();

    // Constants
    void storeConst(ConstantExpression);

    set<ConstantExpression> getAllConst();

    // Statements
    void storeStmt(shared_ptr<Statement>);

    set<shared_ptr<Statement>> getAllStmt();

    // Statement-Statemenet Relations
    void storeRelation(int, int, bool, StmtStmtRelationType);

    bool retrieveRelation(int, int, StmtStmtRelationType);

    vector<int> forwardRetrieveRelation(int, StmtStmtRelationType);

    vector<int> reverseRetrieveRelation(int, StmtStmtRelationType);

    void buildStar(StmtStmtRelationType);

    // Statement-Variable Relations
    void storeRelation(int, string, StmtVarRelationType);

    bool retrieveRelation(int, string, StmtVarRelationType);

    vector<string> forwardRetrieveRelation(int, StmtVarRelationType);

    vector<int> reverseRetrieveRelation(string, StmtVarRelationType);
};