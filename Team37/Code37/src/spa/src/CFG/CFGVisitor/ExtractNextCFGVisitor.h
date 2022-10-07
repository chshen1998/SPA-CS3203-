#ifndef TEAM_37_EXTRACTNEXTCFGVISITOR_H
#define TEAM_37_EXTRACTNEXTCFGVISITOR_H

#include "CFGVisitor.h"

class ExtractNextCFGVisitor : public CFGVisitor, public enable_shared_from_this<CFGVisitor> {
private:
    shared_ptr <Storage> storage = nullptr;
public:
    // Constructor
    ExtractNextCFGVisitor(shared_ptr <Storage> storage);

    // Statements
    void visitSourceCode(shared_ptr <SourceCode> sourceCode);

    void visitProcedure(shared_ptr <Procedure> procedure);


};

#endif
