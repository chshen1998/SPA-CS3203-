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


class Storage {
private:
	shared_ptr<SourceCode> AST;
	set<NameExpression> variables;
	set<ConstantExpression> constants;
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
};