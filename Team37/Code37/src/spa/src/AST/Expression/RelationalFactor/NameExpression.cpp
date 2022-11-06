#include "NameExpression.h"

NameExpression::NameExpression(
    shared_ptr<TNode> parent,
    string varName)
    : RelationalFactor(parent)
    , varName(varName)
{
}

string NameExpression::getVarName()
{
    return this->varName;
}

void NameExpression::accept(shared_ptr<ASTVisitor> visitor)
{
    visitor->visitNameExpression(shared_from_this());
}

string NameExpression::generateString()
{
    return "(" + this->getVarName() + ")";
}
