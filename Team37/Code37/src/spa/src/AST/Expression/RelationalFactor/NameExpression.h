#pragma once

#ifndef TEAM37_NAMEEXPRESSION_H
#define TEAM37_NAMEEXPRESSION_H

using namespace std;

#include "RelationalFactor.h"

#include <string>
#include <memory>

class NameExpression : public RelationalFactor, public enable_shared_from_this<NameExpression> {
private:
    string varName = "";
public:
    NameExpression(shared_ptr<TNode> parent, string varName);

    /**
     * Get the name of the variable in the name expression
     *
     * @return a name
     */
    string getVarName();

    void accept(shared_ptr<ASTVisitor> visitor) override;

    /**
     * Generates a string representing the NameExpression
     * @return a string representation of the NameExpression
     */
    string generateString() override;

    bool operator==(const NameExpression &other) const {
        return varName == other.varName;
    };

    bool operator<(const NameExpression &other) const {
        return varName > other.varName;
    };
};

#endif
