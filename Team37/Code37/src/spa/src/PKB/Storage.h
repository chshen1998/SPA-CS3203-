#pragma once

#ifndef SPA_STORAGE_H
#define SPA_STORAGE_H

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <unordered_set>
#include <memory>
#include <tuple>

#include "../AST/TNode.h"
#include "../AST/Procedure.h"
#include "../AST/SourceCode.h"
#include "../AST/Statement/Statement.h"
#include "../AST/Expression/RelationalFactor/NameExpression.h"
#include "../AST/Expression/RelationalFactor/ConstantExpression.h"
#include "../AST/ASTVisitor/ExtractGeneralASTVisitor.h"
#include "../AST/ASTVisitor/ExtractFollowsASTVisitor.h"
#include "../AST/ASTVisitor/ExtractParentsASTVisitor.h"
#include "../AST/ASTVisitor/ExtractModifiesASTVisitor.h"
#include "../AST/ASTVisitor/ExtractUsesASTVisitor.h"
#include "../AST/ASTVisitor/ExtractCallsASTVisitor.h"
#include "../CFG/CFG.h"

#include "Structures/RelationStorage.h"
#include "Structures/RelationStarStorage.h"

#include "Types/StmtStmtRelationType.h"
#include "Types/StmtVarRelationType.h"
#include "Types/ProcVarRelationType.h"
#include "Types/ProcProcRelationType.h"

using namespace std;

class Storage : public enable_shared_from_this<Storage> {
private:
    shared_ptr<SourceCode> AST;
    shared_ptr<map<int, shared_ptr<CFGNode>>> CFGMap;

    set<NameExpression> variables = {};
    set<ConstantExpression> constants = {};
    set<Procedure> procedures = {};
    set<shared_ptr<Statement>> statements = {};

    // RelationalStore<int, int> Follows = RelationalStore<int, int>();
    RelationStarStorage<int, int> Follows = RelationStarStorage<int, int>();
    RelationStarStorage<int, int> FollowsS = RelationStarStorage<int, int>();
    RelationStarStorage<int, int> Parent = RelationStarStorage<int, int>();
    RelationStarStorage<int, int> ParentS = RelationStarStorage<int, int>();

    RelationStorage<int, string> UsesSV = RelationStorage<int, string>();
    RelationStorage<int, string> ModifiesSV = RelationStorage<int, string>();

    RelationStorage<string, string> UsesPV = RelationStorage<string, string>();
    RelationStorage<string, string> ModifiesPV = RelationStorage<string, string>();

    RelationStarStorage<string, string> Calls = RelationStarStorage<string, string>();
    RelationStarStorage<string, string> CallsS = RelationStarStorage<string, string>();

public:
    // Constructor
    Storage();

    // Queue helper for AST traversal
    // tuple triplet of (line number,container procedure name,called procedure name)
    vector<tuple<int, std::string, std::string>> callStmtProcedureQueue = {};

    // AST
    void storeAST(shared_ptr<SourceCode>);

    shared_ptr<SourceCode> retrieveAST();

    // Variables
    void storeVar(NameExpression);

    set<NameExpression> getAllVar();

    // Constants
    void storeConst(ConstantExpression);

    set<ConstantExpression> getAllConst();

    // Procedure
    void storeProc(Procedure);

    set<Procedure> getAllProc();

    // Statements
    void storeStmt(shared_ptr<Statement>);

    set<shared_ptr<Statement>> getAllStmt();

    void storeCFGMap(shared_ptr<map<int, shared_ptr<CFGNode>>> CFGMap);

    // Post-traversal
    void storeCallStmtProcedure(ProcVarRelationType, StmtVarRelationType);

    // Statement-Statemenet Relations
    void storeRelation(int, int, StmtStmtRelationType);

    bool retrieveRelation(int, int, StmtStmtRelationType);

    vector<int> forwardRetrieveRelation(int, StmtStmtRelationType);

    vector<int> reverseRetrieveRelation(int, StmtStmtRelationType);

    void buildStar(StmtStmtRelationType);

    // Statement-Variable Relations
    void storeRelation(int, std::string, StmtVarRelationType);

    bool retrieveRelation(int, std::string, StmtVarRelationType);

    vector<std::string> forwardRetrieveRelation(int, StmtVarRelationType);

    vector<int> reverseRetrieveRelation(std::string, StmtVarRelationType);

    // Procedure-Variable Relations
    void storeRelation(std::string, std::string, ProcVarRelationType);

    bool retrieveRelation(std::string, std::string, ProcVarRelationType);

    vector<std::string> forwardRetrieveRelation(std::string, ProcVarRelationType);

    vector<std::string> reverseRetrieveRelation(std::string, ProcVarRelationType);

    // Calls
    void storeRelation(std::string, std::string, ProcProcRelationType);

    bool retrieveRelation(std::string, std::string, ProcProcRelationType);

    vector<std::string> forwardRetrieveRelation(std::string, ProcProcRelationType);

    vector<std::string> reverseRetrieveRelation(std::string, ProcProcRelationType);

    void buildStar(ProcProcRelationType);

    // Process Relations(Next/Affects)
    vector<int> forwardComputeRelation(int, StmtStmtRelationType);

    vector<int> backwardComputeRelation(int, StmtStmtRelationType);

    vector<int> getNextStarLineNum(shared_ptr<CFGNode>);
};

#endif