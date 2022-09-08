
#ifndef TEAM_37_CONCRETEASTVISITOR_H
#define TEAM_37_CONCRETEASTVISITOR_H

using namespace std;

#include "AST/ASTVisitor/ASTVisitor.h"
#include <vector>
#include <memory>

class ConcreteASTVisitor : public ASTVisitor {
private:
    vector<shared_ptr<NameExpression>> VisitedTNodeVariables;
    vector<shared_ptr<ConstantExpression>> VisitedConstants;
public:
    // Constructor
    ConcreteASTVisitor();

    // Getters
    vector<shared_ptr<NameExpression>> getTNodeVariables();

    vector<shared_ptr<ConstantExpression>> getVisitedConstants();

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


};


#endif
