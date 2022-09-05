#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "AST/TNode.h"
#include "PKB/PKB.h"
#include "SP/SP.h"
// #include "PQL/QPS.h"

class SIMPLEAnalyser {
private:
	shared_ptr<SP> sourceProcessor;
	shared_ptr<PKB> knowledgeBase;
	// shared_ptr<QPS> queryProcessor;

public:
	// Constructor
	SIMPLEAnalyser() {
		sourceProcessor = make_shared<SP>(new SP());
		knowledgeBase = make_shared<PKB>(new PKB());
		// queryProcessor = make_shared<QPS>(new QPS());
	}

	// Input SIMPLE Program
	// Params program: SIMPLE program to be analysed
	// Returns: Success or error message
	string inputSIMPLE(string program) {
		// TODO: Call sourceProcessor function
	}

	// Excecute Query
	// Params program: Query to be analysed
	// Returns: Result or error message
	string executeQuery(string query) {
		// TODO: Call queryProcessor function
	}
};