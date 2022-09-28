#ifndef SPA_TOKENISER_H
#define SPA_TOKENISER_H

#include <string>
#include <vector>
#include <memory>
using namespace std;

#include "AST/SourceCode.h"
#include "AST/Procedure.h"
#include "AST/Statement/ReadStatement.h"
#include "AST/Statement/PrintStatement.h"
#include "AST/Statement/AssignStatement.h"
#include "AST/Statement/IfStatement.h"
#include "AST/Statement/CallStatement.h"
#include "AST/Expression/RelationalFactor/OperatedExpression.h"
#include "AST/Expression/RelationalFactor/ConstantExpression.h"
#include "AST/Expression/RelationalFactor/NameExpression.h"

class Tokenizer {
public:
    Tokenizer();

    /**
     * Creates ReadStatement object with specified parent.
     * @param line raw read statement, trimmed. First 5 characters are: "read "
     * @param parent parent node of read statement
     * @return ReadStatement object
     */
    static shared_ptr<ReadStatement> tokenizeRead(string line, shared_ptr<TNode> parent);

    /**
     * Creates PrintStatement object with specified parent.
     * @param line print statement in string
     * @param parent parent node of read statement
     * @return PrintStatement object
     */
    static shared_ptr<PrintStatement> tokenizePrint(string line, shared_ptr<TNode> parent);

    /**
     * Creates a AssignStatement object with specified parent.
     * @param line raw assign statement string, no semicolons, trimmed
     * @param parent parent node of assign statement
     * @return AssignStatement object
     */
    static shared_ptr<AssignStatement> tokenizeAssign(string line, shared_ptr<TNode> parent);

    /**
     * Creates a CallStatement object with specified parent.
     * @param line raw call statement string, no semicolons, trimmed
     * @param parent parent node of call statement
     */
    static shared_ptr<CallStatement> tokenizeCall(string line, shared_ptr<TNode> parent);

    /**
     * Creates a RelationalFactor object with the specified parent.
     * @param line a raw relFactor string, trimmed
     * @param parent parent of relFactor
     * @return
     */
    static shared_ptr<RelationalFactor> tokenizeRelFactor(string line);
};

#endif