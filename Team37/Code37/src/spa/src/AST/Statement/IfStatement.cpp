#include "IfStatement.h"

IfStatement::IfStatement(
        shared_ptr<TNode> parent,
        shared_ptr<ConditionalExpression> condExpr)
        : Statement(parent), condExpr(condExpr) { }

void IfStatement::addThenStatement(shared_ptr<Statement> stmt) {
    this->thenStmtLst.push_back(stmt);
}

void IfStatement::addElseStatement(shared_ptr<Statement> stmt) {
    this->elseStmtLst.push_back(stmt);
}

shared_ptr<ConditionalExpression> IfStatement::getConditionalExpression() {
    return this->condExpr;
}

vector<shared_ptr<Statement> > IfStatement::getThenStatements() {
    return this->thenStmtLst;
}

vector<shared_ptr<Statement> > IfStatement::getElseStatements() {
    return this->elseStmtLst;
}

void IfStatement::accept(shared_ptr<ASTVisitor> visitor) {
    visitor->visitIfStatement(shared_from_this());
}

shared_ptr<CFGNode> IfStatement::buildCFG(vector<shared_ptr<CFGNode> > parents) {
    shared_ptr<CFGNode> ifNode = make_shared<CFGNode>(shared_from_this(), parents);
    for (auto p : parents) {
        p->addChild(ifNode);
    }
    vector<shared_ptr<Statement> > thenStmtList = shared_from_this()->getThenStatements();
    vector<shared_ptr<Statement> > elseStmtList = shared_from_this()->getElseStatements();
    vector<shared_ptr<CFGNode> > newParents;
    newParents.push_back(ifNode);
    shared_ptr<CFGNode> cfgNode;

    // create dummy node to always from diamond shape
    vector<shared_ptr<CFGNode> > dummyParents;
    shared_ptr<CFGNode> dummyNode = make_shared<CFGNode>(nullptr, dummyParents);

    // add all then statements
    for (auto s : thenStmtList) {
        cfgNode = s->buildCFG(newParents);
        newParents.clear();
        newParents.push_back(cfgNode);
    }
    cfgNode->addChild(dummyNode);
    dummyNode->addParent(cfgNode);

    // add all else statements
    newParents.clear();
    newParents.push_back(ifNode);
    for (auto s : elseStmtList) {
        cfgNode = s->buildCFG(newParents);
        newParents.clear();
        newParents.push_back(cfgNode);
    }
    cfgNode->addChild(dummyNode);
    dummyNode->addParent(cfgNode);

    return dummyNode;
}
