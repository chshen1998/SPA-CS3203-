#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>

using namespace std;

#include "Storage.h"
#include "Types/StatementType.h"
#include "Types/StmtStmtRelationType.h"
#include "Types/StmtVarRelationType.h"
#include "../AST/Expression/RelationalFactor/ConstantExpression.h"
#include "../AST/Expression/RelationalFactor/NameExpression.h"
#include "../AST/Statement/Statement.h"
#include "../AST/Statement/AssignStatement.h"
#include "../AST/Statement/CallStatement.h"
#include "../AST/Statement/IfStatement.h"
#include "../AST/Statement/PrintStatement.h"
#include "../AST/Statement/ReadStatement.h"
#include "../AST/Statement/WhileStatement.h"

class QueryServicer {
private:
    shared_ptr<Storage> storage;
public:
    // Constructor
    QueryServicer(shared_ptr<Storage>);

    // Query methods

    // Retrieve all
    set<NameExpression> getAllVar();

    set<ConstantExpression> getAllConst();

    set<shared_ptr<Statement>> getAllStmt(StatementType);

    // Stmt Stmt Accessors
    bool retrieveRelation(int, int, StmtStmtRelationType);

    vector<int> forwardRetrieveRelation(int, StmtStmtRelationType);

    vector<int> reverseRetrieveRelation(int, StmtStmtRelationType);

    // Stmt Var Accessors
    bool retrieveRelation(int, string, StmtVarRelationType);

    vector<string> forwardRetrieveRelation(int, StmtVarRelationType);

    vector<int> reverseRetrieveRelation(string, StmtVarRelationType);
};