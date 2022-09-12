#include "Storage.h"
// Constructor
Storage::Storage() {}

/*
Store the AST
@param: AST - Shared Pointer to AST
*/
void Storage::storeAST(shared_ptr<SourceCode> AST) {
	this->AST = AST;
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