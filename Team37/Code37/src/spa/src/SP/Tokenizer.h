#ifndef SPA_TOKENISER_H
#define SPA_TOKENISER_H

#include <memory>
#include <string>
#include <vector>
using namespace std;

#include "AST/Expression/RelationalFactor/ConstantExpression.h"
#include "AST/Expression/RelationalFactor/NameExpression.h"
#include "AST/Expression/RelationalFactor/OperatedExpression.h"
#include "AST/Procedure.h"
#include "AST/SourceCode.h"
#include "AST/Statement/AssignStatement.h"
#include "AST/Statement/CallStatement.h"
#include "AST/Statement/IfStatement.h"
#include "AST/Statement/PrintStatement.h"
#include "AST/Statement/ReadStatement.h"

class Tokenizer {
public:
    Tokenizer();

    /**
     * Creates ReadStatement object with specified parent.
     * @param line raw read statement, no semicolon, trimmed. First 5 characters are: "read "
     * @return ReadStatement object
     */
    static shared_ptr<ReadStatement> tokenizeRead(string line);

    /**
     * Creates PrintStatement object with specified parent.
     * @param line raw print statement, no semicolon, trimmed. First 6 characters are: "print "
     * @return PrintStatement object
     */
    static shared_ptr<PrintStatement> tokenizePrint(string line);

    /**
     * Creates a AssignStatement object with specified parent.
     * @param line raw assign statement string, no semicolon, trimmed
     * @return AssignStatement object
     */
    static shared_ptr<AssignStatement> tokenizeAssign(string line);

    /**
     * Creates a CallStatement object with specified parent.
     * @param line raw call statement string, no semicolon, trimmed
     */
    static shared_ptr<CallStatement> tokenizeCall(string line);

    /**
     * Creates a RelationalFactor object with the specified parent.
     * @param line a raw relFactor string, trimmed
     * @param parent parent of relFactor
     * @return
     */
    static shared_ptr<RelationalFactor> tokenizeRelFactor(string line);
};

#endif