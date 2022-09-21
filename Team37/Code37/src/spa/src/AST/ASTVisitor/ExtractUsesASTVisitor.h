#ifndef TEAM_37_EXTRACTUSESVISITOR_H
#define TEAM_37_EXTRACTUSESVISITOR_H

using namespace std;

#include "AST/ASTVisitor/ASTVisitor.h"

#include <vector>
#include <memory>
#include <string>

class ExtractUsesASTVisitor : public ASTVisitor, public enable_shared_from_this<ASTVisitor> {
private:
    shared_ptr<Storage> storage = nullptr;
public:
    // Constructor
    ExtractUsesASTVisitor(shared_ptr<Storage> storage);

    // Statements
    void visitSourceCode(shared_ptr<SourceCode> sourceCode);

    void visitProcedure(shared_ptr<Procedure> procedure);

    void visitReadStatement(shared_ptr<ReadStatement> readStmt);

    void visitPrintStatement(shared_ptr<PrintStatement> printStmt);

    void visitCallStatement(shared_ptr<CallStatement> callStmt);

    void visitWhileStatement(shared_ptr<WhileStatement> whileStmt);

    virtual void visitIfStatement(shared_ptr<IfStatement> ifStmt);

    void visitAssignStatement(shared_ptr<AssignStatement> assignStmt);

    // RelationalFactor
    void visitNameExpression(shared_ptr<NameExpression> nameExpr);

    void visitConstantExpression(shared_ptr<ConstantExpression> constantExpr);

    void visitOperatedExpression(shared_ptr<OperatedExpression> operatedExpr);

    // ConditionalExpression
    void visitRelationalExpression(shared_ptr<RelationalExpression> relationalExpr);

    void visitNotCondition(shared_ptr<NotCondition> notCondition);

    void visitAndCondition(shared_ptr<AndCondition> andCondition);

    void visitOrCondition(shared_ptr<OrCondition> orCondition);

    void visitParentAndStore(shared_ptr<TNode> node, string variable);

};


#endif
