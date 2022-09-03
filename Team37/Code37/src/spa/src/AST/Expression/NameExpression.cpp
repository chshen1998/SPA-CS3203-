#include "NameExpression.h"

NameExpression::NameExpression(string varName) : varName(varName) { }

string NameExpression::getVarName() {
    return this->varName;
}
