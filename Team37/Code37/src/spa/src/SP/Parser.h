#ifndef SPA_PARSER_H
#define SPA_PARSER_H

using namespace std;

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <memory>

#include "AST/SourceCode.h"
#include "AST/Procedure.h"
#include "AST/Expression/ConditionalExpression/ConditionalExpression.h"
#include "AST/Expression/ConditionalExpression/RelationalExpression.h"
#include "AST/Statement/WhileStatement.h"
#include "AST/Statement/IfStatement.h"
#include "AST/Expression/ConditionalExpression/NotCondition.h"
#include "AST/Expression/ConditionalExpression/AndCondition.h"
#include "AST/Expression/ConditionalExpression/OrCondition.h"

class Parser {
public:
    // TODO: Update method description after fixing the method
    /**
     * Extracts each procedure as a string. Ultimately produces ProcedureList.
     * @param srcCode source code as a string
     * @param procedures vector to store procedures as strings.
     * @return ProcedureList which is a vector containing procedures as strings
     */
    static vector<string> extractProcedures(string srcCode, vector<string> procedures);

    /**
     * Extracts the name of a procedure.
     * @param procedure raw procedure string, inclusive of "procedure" keyword and brackets "{...}"
     * @return name of the procedure
     */
    static string extractProcName(string procedure);

    /**
     * Extracts statements of a procedure as strings.
     * @param procedure a raw list of statements
     * @param statementList a predeclared vector of statements so that we can do recursion with it
     * @return StatementList which is a vector containing statements (without semicolon) as strings.
     */
    static vector<string> extractStatements(string procedure, vector<string> statementList);

    /**
     * Parses an if or while statement string and extracts the conditional
     * expression as a string that sits between the first set of parentheses ( )
     * @param str an if or while statement string that contains a conditional expression
     * @return the conditional expression as a string without the bracket wrapper
     */
    static string extractConditionalExpr(string str);

    /**
     * Takes in a raw procedure string and removes the keyword, procedure name, and brackets
     * @param procedure a raw procedure string
     * @return a raw string of the statements in the procedure
     */
    static string removeProcedureWrapper(string procedure);

    /**
     * Parses raw sourceCode string
     * @param srcCode raw sourcecode string
     * @param filename the filename of the SIMPLE source code
     * @return a SourceCode node
     */
    static shared_ptr<SourceCode> parseSourceCode(string srcCode, string filename);

    /**
     * Parses raw procedure string
     * @param procedure unprocessed procedure string
     * @return a Procedure node
     */
    static shared_ptr<Procedure> parseProcedure(string procedure);

    /**
     * Parses an unprocessed statement string
     * @param statement unprocessed statement string
     * @param parentNode parent node of the statement
     * @return a Statement node
     */
    static shared_ptr<Statement> parseStatement(string statement);

    /**
     * Parses raw if-else block string
     * @param ifElseBlock if-else block string
     * @param parent parent node of the if statement
     * @return an IfStatement node
     */
    static shared_ptr<IfStatement> parseIfElse(string ifElseBlock);

    /**
     * Parses raw while block string
     * @param whileBlock while block string
     * @param parent parent node of the while statement
     * @return a WhileStatement node
     */
    static shared_ptr<WhileStatement> parseWhile(string whileBlock);

    /**
     * Parses raw conditional expression string
     * @param condExprStr raw conditional expression string
     * @param parent parent node of the conditional expression
     * @return a ConditionalExpression node
     */
    static shared_ptr<ConditionalExpression> parseCondExpr(string condExprStr);

    /**
     * Parses a raw relational expression string
     * @param relExprStr raw relational expression string
     * @param parent parent node of the relational expression
     * @return a RelationalExpression node
     */
    static shared_ptr<RelationalExpression> parseRelExpr(string relExprStr);
};

#endif //SPA_PARSER_H
