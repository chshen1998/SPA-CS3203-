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
@param varNode Shared pointers to a NameExpression Node
*/
void Storage::storeVar(NameExpression varNode) {
	(this->variables).insert(varNode);
}

/* 
Retrieve all stored variables
@returns Set of shared pointers of Variables
*/
set<NameExpression> Storage::getAllVar() {
	return this->variables;
 }


// Constant
/*
Store a constant
@param constNode Shared pointers to a ConstantExpression Node
*/
void Storage::storeConst(ConstantExpression constNode) {
	(this->constants).insert(constNode);
}

/*
Retrieve all stored constants
@return Set of shared pointers of constants stored
*/
set<ConstantExpression> Storage::getAllConst() {
	return this->constants;
}