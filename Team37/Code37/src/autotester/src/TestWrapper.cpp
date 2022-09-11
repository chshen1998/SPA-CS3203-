#include "TestWrapper.h"

// implementation code of WrapperFactory - do NOT modify the next 5 lines
AbstractWrapper* WrapperFactory::wrapper = 0;
AbstractWrapper* WrapperFactory::createWrapper() {
  if (wrapper == 0) wrapper = new TestWrapper;
  return wrapper;
}
// Do not modify the following line
volatile bool AbstractWrapper::GlobalStop = false;

// a default constructor
TestWrapper::TestWrapper() {
	sourceProcessor = make_shared<SP>();
	knowledgeBase = make_shared<PKB>();
	queryProcessor = make_shared<QPS>();
}

// destructor
TestWrapper::~TestWrapper() {
	sourceProcessor.reset();
	knowledgeBase.reset();
	queryProcessor.reset();
}

// method for parsing the SIMPLE source
void TestWrapper::parse(string filename) {
	shared_ptr<SourceCode> AST = SP::parse(filename);
	knowledgeBase->buildFromAst(AST);
}

// method to evaluating a query
void TestWrapper::evaluate(string query, list<string>& results){
	// results = queryProcessor->processQuery(query);
}
