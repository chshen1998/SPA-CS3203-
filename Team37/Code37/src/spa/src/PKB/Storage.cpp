#include "Storage.h"

// Constructor
Storage::Storage() {}

/*
Store the AST
@param: AST - Shared Pointer to AST
*/
void Storage::storeAST(shared_ptr<SourceCode> AST) {
    this->AST = AST;

    // Get AST statement num and intialise Array2D
    Follows = Array2D(AST->getNumOfStatements());
    Parent = Array2D(AST->getNumOfStatements());

    // initialising AST visitors
    shared_ptr<ExtractGeneralASTVisitor> generalAstVisitor = make_shared<ExtractGeneralASTVisitor>(shared_from_this());
    shared_ptr<ExtractParentsASTVisitor> parentsAstVisitor = make_shared<ExtractParentsASTVisitor>(shared_from_this());
    shared_ptr<ExtractFollowsASTVisitor> followsAstVisitor = make_shared<ExtractFollowsASTVisitor>(shared_from_this());
    shared_ptr<ExtractModifiesASTVisitor> modifiesAstVisitor = make_shared<ExtractModifiesASTVisitor>(
            shared_from_this());
    shared_ptr<ExtractUsesASTVisitor> usesAstVisitor = make_shared<ExtractUsesASTVisitor>(
            shared_from_this());

    // we start by traversing the AST using a Extract AST Visitor
    AST->accept(generalAstVisitor);

    // traverse the AST using a parents AST Visitor
    AST->accept(parentsAstVisitor);
    this->buildStar(PARENT);

    // traverse the AST using a follows AST Visitor
    AST->accept(followsAstVisitor);
    this->buildStar(FOLLOWS);

    // traverse the AST using a modifies AST Visitor
    AST->accept(modifiesAstVisitor);
    this->storeCallStmtProcedure(MODIFIESPV, MODIFIESSV);

    // traverse the AST using a uses AST Visitor
    AST->accept(usesAstVisitor);
    this->storeCallStmtProcedure(USESPV, USESSV);

}

/*
Retrieve Stored AST
@return AST SourceCode node if AST added, nullptr otherwise
*/
shared_ptr<SourceCode> Storage::retrieveAST() {
    return this->AST;
}

// Variable
/*
Store a variable in the variable set
@param varNode NameExpression Node to store
*/
void Storage::storeVar(NameExpression varNode) {
    (this->variables).insert(varNode);
}

/* 
Retrieve all stored variables
@returns Set of Variables
*/
set<NameExpression> Storage::getAllVar() {
    return this->variables;
}


// Constant
/*
Store a constant
@param constNode a ConstantExpression Node
*/
void Storage::storeConst(ConstantExpression constNode) {
    (this->constants).insert(constNode);
}

/*
Retrieve all stored constants
@return Set of constants stored
*/
set<ConstantExpression> Storage::getAllConst() {
    return this->constants;
}

// Procedure
/*
Store a procedure
@param constNode a Procedure Node
*/
void Storage::storeProc(Procedure procNode) {
    (this->procedures).insert(procNode);
}

/*
Retrieve all stored procedure
@return Set of procedure stored
*/
set<Procedure> Storage::getAllProc() {
    return this->procedures;
}

// Statement
/*
Store a shared pointer to a statement
@param stmtNode Shared pointers to a Statement Node
*/
void Storage::storeStmt(shared_ptr<Statement> stmtNode) {
    (this->statements).insert(stmtNode);
}

/*
Retrieve all stored statements
@return Set of shared pointers of statements stored
*/
set<shared_ptr<Statement>> Storage::getAllStmt() {
    return this->statements;
}

/**
 * after traversing the AST we have to store the Uses(c,v) relations that was not handled in the queue
 */
void Storage::storeCallStmtProcedure(ProcVarRelationType relationProcType, StmtVarRelationType relationStmtType) {
    for (const auto &tuple: this->callStmtProcedureQueue) {
        int lineNum = get<0>(tuple);
        string parentProcedureName = get<1>(tuple);
        string calledProcedureName = get<2>(tuple);

        vector<string> storedVariablesInProcedure = this->forwardRetrieveRelation(calledProcedureName,
                                                                                  relationProcType);
        for (const auto &variable: storedVariablesInProcedure) {

            // store Uses(c,v)
            this->storeRelation(lineNum, variable, relationStmtType);
            this->storeRelation(parentProcedureName, variable, relationProcType);
        }
    }

// empty the queue after storing the relations
    this->callStmtProcedureQueue = {};
}

/*
Store Relation of a Statement-Statement Relationship (Non-star). For Relation(stmt1, stmt2)
@param stmt1 First statement
@param stmt2 Second statement
@param value Value of relation
@param type Type of relation
*/
void Storage::storeRelation(int stmt1, int stmt2, bool value, StmtStmtRelationType type) {
    switch (type) {
        case (FOLLOWS):
            Follows.store(stmt1, stmt2, value);
            break;
        case (PARENT):
            Parent.store(stmt1, stmt2, value);
            break;
        default:
            throw invalid_argument("Not a Statement-Statement Realtion");
    }
}

/*
Retrieve Statement-Statement Relation Stored. For Relation(stmt1, stmt2)
@param stmt1
@param stmt2
@param type Type of relation
@returns Value of relation stored
*/
bool Storage::retrieveRelation(int stmt1, int stmt2, StmtStmtRelationType type) {
    switch (type) {
        case (FOLLOWS):
            return Follows.retrieve(stmt1, stmt2);
            break;
        case (FOLLOWSS):
            return FollowsS.retrieve(stmt1, stmt2);
            break;
        case (PARENT):
            return Parent.retrieve(stmt1, stmt2);
            break;
        case (PARENTS):
            return ParentS.retrieve(stmt1, stmt2);
            break;
        default:
            throw invalid_argument("Not a Statement-Statement Realtion");
    }
}

/*
Retrieve Reverse Relation Stored. For Relation(stmt1, stmt2)
@param stmt2
@param type Type of relation
@returns All stmt1 such that Relation(stmt1, stmt2) is True
*/
vector<int> Storage::reverseRetrieveRelation(int stmt2, StmtStmtRelationType type) {
    switch (type) {
        case (FOLLOWS):
            return Follows.reverseRetrieve(stmt2);
            break;
        case (FOLLOWSS):
            return FollowsS.reverseRetrieve(stmt2);
            break;
        case (PARENT):
            return Parent.reverseRetrieve(stmt2);
            break;
        case (PARENTS):
            return ParentS.reverseRetrieve(stmt2);
            break;
        default:
            throw invalid_argument("Not a Statement-Statement Realtion");
    }
}

/*
Retrieve Forward Relation Stored. For Relation(stmt1, stmt2)
@param stmt1
@param type Type of relation
@returns All stmt2 such that Relation(stmt1, stmt2) is True
*/
vector<int> Storage::forwardRetrieveRelation(int stmt1, StmtStmtRelationType type) {
    switch (type) {
        case (FOLLOWS):
            return Follows.forwardRetrieve(stmt1);
            break;
        case (FOLLOWSS):
            return FollowsS.forwardRetrieve(stmt1);
            break;
        case (PARENT):
            return Parent.forwardRetrieve(stmt1);
            break;
        case (PARENTS):
            return ParentS.forwardRetrieve(stmt1);
            break;
        default:
            throw invalid_argument("Not a Statement-Statement Realtion");
    }
    switch (type) {
        case (FOLLOWS):
            return Follows.forwardRetrieve(stmt1);
            break;
        case (FOLLOWSS):
            return FollowsS.forwardRetrieve(stmt1);
            break;
        case (PARENT):
            return Parent.forwardRetrieve(stmt1);
            break;
        case (PARENTS):
            return ParentS.forwardRetrieve(stmt1);
            break;
        default:
            throw invalid_argument("Not a Statement-Statement Relation");
    }
}


/**
Only called after filling up Statement-Statement Array2D, fills in the Star Array2D
@param type Type of relation

*/
void Storage::buildStar(StmtStmtRelationType type) {
    switch (type) {
        case (FOLLOWS):
            FollowsS = Follows.buildStar();
            break;
        case (PARENT):
            ParentS = Parent.buildStar();
            break;
        default:
            throw invalid_argument("Not a Statement-Statement Realtion");
    }
}

/*
Store Relation of a Statement-Variable Relationship. For Relation(stmt, var)
@param stmt Statement Number
@param var Variable Name
@param type Type of relation
*/
void Storage::storeRelation(int stmt, string var, StmtVarRelationType type) {
    switch (type) {
        case (USESSV):
            UsesSV.store(stmt, var);
            break;
        case (MODIFIESSV):
            ModifiesSV.store(stmt, var);
            break;
        default:
            throw invalid_argument("Not a Statement-Variable Relation");
    }
}

/*
Retrieve Statement-Statement Relation Stored. For Relation(stmt, var)
@param stmt
@param var
@param type Type of relation
@returns Value of relation stored
*/
bool Storage::retrieveRelation(int stmt, string var, StmtVarRelationType type) {
    switch (type) {
        case (USESSV):
            return UsesSV.retrieve(stmt, var);
            break;
        case (MODIFIESSV):
            return ModifiesSV.retrieve(stmt, var);
            break;
        default:
            throw invalid_argument("Not a Statement-Variable Realtion");
    }
}

/*
Retrieve Forward Relation Stored.For Relation(stmt, var)
@param stmt
@param type Type of relation
@returns All var such that Relation(stmt, var) is True
*/
vector<string> Storage::forwardRetrieveRelation(int stmt, StmtVarRelationType type) {
    switch (type) {
        case (USESSV):
            return UsesSV.forwardRetrieve(stmt);
            break;
        case (MODIFIESSV):
            return ModifiesSV.forwardRetrieve(stmt);
            break;
        default:
            throw invalid_argument("Not a Statement-Variable Relation");
    }
}

/*
Retrieve Reverse Relation Stored.For Relation(stmt, var)
@param var
@param type Type of relation
@returns All stmt such that Relation(stmt, var) is True
*/
vector<int> Storage::reverseRetrieveRelation(string var, StmtVarRelationType type) {
    switch (type) {
        case (USESSV):
            return UsesSV.reverseRetrieve(var);
            break;
        case (MODIFIESSV):
            return ModifiesSV.reverseRetrieve(var);
            break;
        default:
            throw invalid_argument("Not a Statement-Variable Relation");
    }
}

/*
Store Relation of a Procedure-Variable Relationship. For Relation(proc, var)
@param proc Procedure Name
@param var Variable Name
@param type Type of relation
*/
void Storage::storeRelation(string proc, string var, ProcVarRelationType type) {
    switch (type) {
        case (USESPV):
            UsesPV.store(proc, var);
            break;
        case (MODIFIESPV):
            ModifiesPV.store(proc, var);
            break;
        default:
            throw invalid_argument("Not a Procedure-Variable Relation");
    }
}

/*
Retrieve Procedure-Variable Relationship. For Relation(proc, var)
@param proc Procedure Name
@param var Variable Name
@param type Type of relation
@returns Value of relation stored
*/
bool Storage::retrieveRelation(string proc, string var, ProcVarRelationType type) {
    switch (type) {
        case (USESPV):
            return UsesPV.retrieve(proc, var);
            break;
        case (MODIFIESPV):
            return ModifiesPV.retrieve(proc, var);
            break;
        default:
            throw invalid_argument("Not a Procedure-Variable Realtion");
    }
}

/*
Retrieve Forward Relation Stored. For Relation(proc, var)
@param proc Procedure Name
@param type Type of relation
@returns All var such that Relation(stmt, var) is True
*/
vector<string> Storage::forwardRetrieveRelation(string proc, ProcVarRelationType type) {
    switch (type) {
        case (USESSV):
            return UsesPV.forwardRetrieve(proc);
            break;
        case (MODIFIESSV):
            return ModifiesPV.forwardRetrieve(proc);
            break;
        default:
            throw invalid_argument("Not a Procedure-Variable Relation");
    }
}

/*
Retrieve Reverse Relation Stored. For Relation(proc, var)
@param var Variable Name
@param type Type of relation
@returns All stmt such that Relation(stmt, var) is True
*/
vector<string> Storage::reverseRetrieveRelation(string var, ProcVarRelationType type) {
    switch (type) {
        case (USESSV):
            return UsesPV.reverseRetrieve(var);
            break;
        case (MODIFIESSV):
            return ModifiesPV.reverseRetrieve(var);
            break;
        default:
            throw invalid_argument("Not a Procedure-Variable Relation");
    }
}