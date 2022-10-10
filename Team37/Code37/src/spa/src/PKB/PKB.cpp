#include "PKB.h"

PKB::PKB() {
    storage = make_shared<Storage>();
    queryServicer = make_shared<QueryServicer>(storage);
}


shared_ptr<QueryServicer> PKB::getQueryServicer() {
    return this->queryServicer;
}

void PKB::buildFromAst(shared_ptr<SourceCode> AST) {
    this->storage->storeAST(AST);
};

void PKB::buildFromCFG(shared_ptr<map<int, shared_ptr<CFGNode>>> CFGMap) {
    this->storage->storeCFGMap(CFGMap);
}



