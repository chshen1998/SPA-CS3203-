#ifndef TEAM_37_CFGVISITOR_H
#define TEAM_37_CFGVISITOR_H
#pragma once

using namespace std;

#include <memory>

class Storage;

class SourceCode;

class Procedure;

class ReadStatement;

class PrintStatement;

class CallStatement;

class WhileStatement;

class IfStatement;

class AssignStatement;

class NameExpression;

class ConstantExpression;

class OperatedExpression;

class RelationalExpression;

class NotCondition;

class AndCondition;

class OrCondition;

class TNode;

class CFGNode;

class CFGVisitor {
public:
    virtual void visitSourceCode(shared_ptr<SourceCode> sourceCode) = 0;

    virtual void visitProcedure(shared_ptr<Procedure> procedure) = 0;

    virtual void visitCFGNode(shared_ptr<CFGNode> cfgNode) = 0;

};

#endif
