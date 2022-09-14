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
	Follows = Array2D(AST->getNumStmts());
	Parent = Array2D(AST->getNumStmts());

    // We start by traversing the AST using a Extract AST Visitor
    shared_ptr<ExtractASTVisitor> visitor = make_shared<ExtractASTVisitor>(shared_from_this());
    AST->accept(visitor);
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
@return Set of of constants stored
*/
set<ConstantExpression> Storage::getAllConst() {
	return this->constants;
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
@return Set of shared pointers of statments stored
*/
set<shared_ptr<Statement>> Storage::getAllStmt() {
	return this->statements;
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
	case(FOLLOWS):
		Follows.store(stmt1, stmt2, value);
		break;
	case(PARENT):
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
	case(FOLLOWS):
		return Follows.retrieve(stmt1, stmt2);
		break;
	case(FOLLOWSS):
		return FollowsS.retrieve(stmt1, stmt2);
		break;
	case(PARENT):
		return Parent.retrieve(stmt1, stmt2);
		break;
	case(PARENTS):
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
	case(FOLLOWS):
		return Follows.reverseRetrieve(stmt2);
		break;
	case(FOLLOWSS):
		return FollowsS.reverseRetrieve(stmt2);
		break;
	case(PARENT):
		return Parent.reverseRetrieve(stmt2);
		break;
	case(PARENTS):
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
@returns All stmt2 such that Relatioin(stmt1, stmt2) is True
*/
vector<int> Storage::forwardRetrieveRelation(int stmt1, StmtStmtRelationType type) {
	switch (type) {
	case(FOLLOWS):
		return Follows.forwardRetrieve(stmt1);
		break;
	case(FOLLOWSS):
		return FollowsS.forwardRetrieve(stmt1);
		break;
	case(PARENT):
		return Parent.forwardRetrieve(stmt1);
		break;
	case(PARENTS):
		return ParentS.forwardRetrieve(stmt1);
		break;
	default:
		throw invalid_argument("Not a Statement-Statement Realtion");
	}
}


/**
Only called after filling up Statement-Statement Array2D, fills in the Star Array2D
@param type Type of relation

*/
void Storage::buildStar(StmtStmtRelationType type) {
	switch (type) {
	case(FOLLOWS):
		FollowsS =  Follows.buildStar();
		break;
	case(PARENT):
		ParentS = Parent.buildStar();
		break;
	default:
		throw invalid_argument("Not a Statement-Statement Realtion");
	}
}