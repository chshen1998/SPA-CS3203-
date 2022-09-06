#include "NameExpression.h"

NameExpression::NameExpression(
        shared_ptr<TNode> parent,
        string varName) : RelationalFactor(parent), varName(varName) { }

string NameExpression::getVarName() {
    return this->varName;
}
