#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <memory>

using namespace std;

#include "QueryServicer.h"
#include "Storage.h"
#include "ElementType.h"
#include "../AST/Expression/RelationalFactor/ConstantExpression.h"
#include "../AST/Expression/RelationalFactor/NameExpression.h"

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

};