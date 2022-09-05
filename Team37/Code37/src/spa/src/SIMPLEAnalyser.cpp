#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "AST/TNode.h"
#include "QPS/QPS.h"
#include "PKB/PKB.h"
#include "SP/SP.h"

class SIMPLEAnalyser {
private:
	shared_ptr<SP> sourceProcessor;
	shared_ptr<PKB> knowledgeBase;
	shared_ptr<QPS> queryProcessor;

public:
	// Constructor
	SIMPLEAnalyser() {
		sourceProcessor = make_shared<SP>(SP());
		knowledgeBase = make_shared<PKB>(PKB());
		queryProcessor = make_shared<QPS>(QPS());
	}

	// Input SIMPLE Program
	// Params program: SIMPLE program to be analysed
	// Returns: Success or error message
	shared_ptr<TNode> inputSIMPLE(string program) {
		// TODO: Call sourceProcessor function
		shared_ptr<TNode> AST = SP::parse(program);
		// knowledgeBase -> buildFromAst(AST)
	}

	// Excecute Query
	// Params program: Query to be analysed
	// Returns: Result or error message
	string executeQuery(string query) {
		// TODO: Call queryProcessor function
		//return queryProcessor->processQuery(query);
		set<string> result = knowledgeBase->retrieveAll();

		string output = "";

		set<string>::iterator itr;

		for (itr = result.begin(); itr != result.end(); itr++)	{
			output += *itr;
		}
	}
};