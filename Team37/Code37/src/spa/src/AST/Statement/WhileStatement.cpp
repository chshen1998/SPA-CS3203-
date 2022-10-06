#include "WhileStatement.h"

WhileStatement::WhileStatement(
        shared_ptr<TNode> parent,
        shared_ptr<ConditionalExpression> condExpr
        ) : Statement(parent), condExpr(condExpr) { }

void WhileStatement::addStatement(shared_ptr<Statement> stmt) {
    WhileStatement::stmtLst.push_back(stmt);
}

shared_ptr<ConditionalExpression> WhileStatement::getConditionalExpression() {
    return WhileStatement::condExpr;
}

vector<shared_ptr<Statement> > WhileStatement::getStatements() {
    return WhileStatement::stmtLst;
}

void WhileStatement::accept(shared_ptr<ASTVisitor> visitor) {
    visitor->visitWhileStatement(shared_from_this());
}

shared_ptr<CFGNode> WhileStatement::buildCFG(vector<shared_ptr<CFGNode> > parents, shared_ptr<CFG> cfg) {
    shared_ptr<CFGNode> whileNode = make_shared<CFGNode>(shared_from_this(), parents);

    //store mapping of while statement
    cfg->addMapping(this->getLineNum(), whileNode);

    for (auto p : parents) {
        p->addChild(whileNode);
    }
    vector<shared_ptr<Statement> > stmtLst = shared_from_this()->getStatements();
    vector<shared_ptr<CFGNode> > newParents;
    newParents.push_back(whileNode);
    shared_ptr<CFGNode> cfgNode;
    for (auto s : stmtLst) {
        cfgNode = s->buildCFG(newParents, cfg);
        newParents.clear();
        newParents.push_back(cfgNode);
    }
    // link last cfg node back to while node
    cfgNode->addChild(whileNode); // child 0 of cfg node is while node
    whileNode->addParent(cfgNode);

    // create dummy node to continue from while node and last statement in while loop
    vector<shared_ptr<CFGNode> > dummyParents;
    shared_ptr<CFGNode> dummyNode = make_shared<CFGNode>(nullptr, dummyParents);
    cfgNode->addChild(dummyNode); //child 1 of cfg node is dummy node
    dummyNode->addParent(cfgNode); //dummy node parents[0] is cfg node before it
    whileNode->addChild(dummyNode);
    dummyNode->addParent(whileNode); //dummy node parents[1] is while node

    //store while statement in dummy node
    dummyNode->setStmtToStore(whileNode);
    return dummyNode;
}