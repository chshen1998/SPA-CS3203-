#include "TestWrapper.h"

// implementation code of WrapperFactory - do NOT modify the next 5 lines
AbstractWrapper *WrapperFactory::wrapper = 0;

AbstractWrapper *WrapperFactory::createWrapper() {
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
    try {
        printf("%s\n", "Before Parse");

        shared_ptr<SourceCode> AST = SP::parse(filename);
        printf("%s\n", "After Parse");

        vector<shared_ptr<CFG> > cfgLst = AST->getAllCFGs();

        printf("%s\n", "After CFG");

        shared_ptr<map<int, shared_ptr<CFGNode> > > allCFGMaps = AST->getAllCFGMaps();
        knowledgeBase->buildFromAst(AST);
        knowledgeBase->buildFromCFG(allCFGMaps);
        queryProcessor->setQueryServicer(knowledgeBase->getQueryServicer());

        printf("%s\n", "After PKB");

    } catch (InvalidSyntaxException e) {
        printf("%s\n", e.what());
        exit(0);
    }
    
}

// method to evaluating a query
void TestWrapper::evaluate(string query, list<string> &results) {
    printf("%s\n", "Reached QPS");
    queryProcessor->evaluate(query, results);
}
