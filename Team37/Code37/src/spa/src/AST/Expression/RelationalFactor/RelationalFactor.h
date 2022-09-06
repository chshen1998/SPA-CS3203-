#ifndef SPA_RELATIONALFACTOR_H
#define SPA_RELATIONALFACTOR_H

#include "../Expression.h"

class RelationalFactor : public Expression {
public:
    RelationalFactor(shared_ptr<TNode> parent) : Expression(parent) { }
};

#endif
