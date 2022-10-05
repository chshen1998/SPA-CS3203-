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

shared_ptr<CFGNode> WhileStatement::buildCFG(vector<shared_ptr<CFGNode> > parents) {
    shared_ptr<CFGNode> whileNode = make_shared<CFGNode>(shared_from_this(), parents);
    for (auto p : parents) {
        p->addChild(whileNode);
    }
    vector<shared_ptr<Statement> > stmtLst = shared_from_this()->getStatements();
    vector<shared_ptr<CFGNode> > newParents;
    newParents.push_back(whileNode);
    shared_ptr<CFGNode> cfgNode;
    for (auto s : stmtLst) {
        cfgNode = s->buildCFG(newParents);
        newParents.clear();
        newParents.push_back(cfgNode);
    }
    // link last cfg node back to while node
    cfgNode->addChild(whileNode);
    whileNode->addParent(cfgNode);
    return whileNode;
}