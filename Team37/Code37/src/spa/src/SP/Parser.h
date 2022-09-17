#ifndef SPA_PARSER_H
#define SPA_PARSER_H

using namespace std;

#include <string>
#include <vector>
#include <iostream>

#include "AST/SourceCode.h"
#include "AST/Procedure.h"
#include "AST/Expression/ConditionalExpression/ConditionalExpression.h"
#include "AST/Statement/WhileStatement.h"
#include "AST/Statement/IfStatement.h"
#include "AST/Expression/ConditionalExpression/NotCondition.h"

class Parser {
public:
    /**
     * Extracts each procedure as a string. Ultimately produces ProcedureList.
     * @param srcCode source code as a string
     * @param procedures vector to store procedures as strings.
     * @return ProcedureList which is a vector containing procedures as strings
     */
    static vector<string> extractProcedures(string srcCode, vector<string> procedures);

    /**
     * Extracts the name of the procedure.
     * @param procedure unprocessed procedure string
     * @return names of the procedure
     */
    static string extractProcName(string procedure);

    /**
     * Takes in a raw procedure string and only returns the statements within the curly braces of the procedure
     * @param procedure a raw procedure string
     * @return a raw string of the statements in the procedure
     */
    static string removeProcedureWrapper(string procedure);

    /**
     * Extracts statements of a procedure as strings.
     * @param procedure a raw list of statements
     * @param statementList a predeclared vector of statements so that we can do recursion with it
     * @return StatementList which is a vector containing statements (without semicolon) as strings.
     */
    static vector<string> extractStatements(string procedure, vector<string> statementList);

    /**
     * Parses an unprocessed statement string and calls relevant Tokenizer method
     * for that statement type.
     * @param statement unprocessed statement string
     * @param parentNode parent node of the statement
     * @return a Statement node
     */
    static shared_ptr<Statement> parseStatement(string statement, shared_ptr<TNode> parentNode);

    /**
     * Parses an unprocessed procedure string and produces a statement list which
     * is a vector containing unprocessed statement strings.
     * @param procedure unprocessed procedure string
     * @param srcCodeNode parent SourceCode node of the procedure
     * @return a Procedure node
     */
    static shared_ptr<Procedure> parseProcedure(string procedure, shared_ptr<SourceCode> srcCodeNode);

    /**
     * Parses unprocessed source code string and produces a procedure list
     * which is a vector containing unprocessed procedures as strings.
     * @param srcCode sourcecode which has been converted to a string
     * @param filepath the filepath of the SIMPLE source code
     * @return a SourceCode node
     */
    static shared_ptr<SourceCode> parseSourceCode(string srcCode, string filepath);

    /**
     * Parses unprocessed if-else block string and produces a statement list
     * which is a vector containing unprocessed statements as strings.
     * @param ifElseBlock if-else block string
     * @param parent parent node of the if statement
     * @return an IfStatement node
     */
    static shared_ptr<IfStatement> parseIfElse(string ifElseBlock, shared_ptr<TNode> parent);
    /**
     * Parses unprocessed while block string and produces a statement list
     * which is a vector containing unprocessed statements as strings.
     * @param whileBlock while block string
     * @param parent parent node of the while statement
     * @return a WhileStatement node
     */
    static shared_ptr<WhileStatement> parseWhile(string whileBlock, shared_ptr<TNode> parent);

    /**
     * Parses unprocessed conditional expression string
     * @param condExprStr conditional expression string
     * @param parent parent node of the conditional expression
     * @return a ConditionalExpression node
     */
    static shared_ptr<ConditionalExpression> parseCondExpr(string condExprStr, shared_ptr<TNode> parent);

    /**
     * Parses an if or while block string and extracts the conditional
     * expression as a string that sits between the first set of parentheses ( )
     * @param block an if or while block that contains a
     * conditional expression
     * @param firstEgyptianOpen index of the first egyptian open bracket
     * @return the conditional expression as a string
     */
    static string extractConditionalExpr(string block, size_t firstEgyptianOpen);

    /**
     * Parses a block of code and extracts a statement block that sits between
     * a pair of open and closed egyptian brackets
     * @param block SIMPLE code block as a string
     * @param firstEgyptianOpen index of the first open egyptian bracket
     * @return a statement block of all statements as one string
     */
    static string extractStatementBlock(string block, size_t firstEgyptianOpen);


};


#endif //SPA_PARSER_H
