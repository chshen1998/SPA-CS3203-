#include "Storage.h"

// Constructor
Storage::Storage() {}

/*
Store the AST
@param: AST - Shared Pointer to AST
*/
void Storage::storeAST(shared_ptr<TNode> AST) {
    this->AST = AST;

    // We start by traversing the AST using a Concrete Visitor
    shared_ptr<ConcreteASTVisitor> visitor = make_shared<ConcreteASTVisitor>();
    AST->accept(visitor);

    // After traversing we store the variables,constants and relationships
    // Storing variables
    for (auto variable: visitor->getTNodeVariables()) {
        this->storeVar(variable);
    }

    // Storing constants
    for (auto constant: visitor->getVisitedConstants()) {
        this->storeConst(constant);
    }

    // Storing relationships
}


/*
Retrieve Stored AST
@return AST SourceCode node if AST added, nullptr otherwise
*/
shared_ptr<TNode> Storage::retrieveAST() {
    return this->AST;
}

// Variable
/*
Store a variable in the variable set
@param varNode Shared pointers to a NameExpression Node
*/
shared_ptr<TNode> Storage::storeVar(shared_ptr<NameExpression> varNode) {
    (this->variables).insert(varNode);
}

/*
Retrieve all stored variables
@returns Set of shared pointers of Variables
*/
set<shared_ptr<TNode>> Storage::getAllVar() {
    return this->variables;
}


// Constant
/*
Store a constant
@param constNode Shared pointers to a ConstantExpression Node
*/
void Storage::storeConst(shared_ptr<ConstantExpression> constNode) {
    (this->constants).insert(constNode);
}

/*
Retrieve all stored constants
@return Set of shared pointers of constants stored
*/
set<shared_ptr<TNode>> Storage::getAllConst() {
    return this->constants;
}