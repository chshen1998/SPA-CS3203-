#ifndef SPA_PARSER_H
#define SPA_PARSER_H

using namespace std;

#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "AST/Expression/ConditionalExpression/AndCondition.h"
#include "AST/Expression/ConditionalExpression/ConditionalExpression.h"
#include "AST/Expression/ConditionalExpression/NotCondition.h"
#include "AST/Expression/ConditionalExpression/OrCondition.h"
#include "AST/Expression/ConditionalExpression/RelationalExpression.h"
#include "AST/Procedure.h"
#include "AST/SourceCode.h"
#include "AST/Statement/IfStatement.h"
#include "AST/Statement/WhileStatement.h"

class Parser {
public:
    /**
     * Extracts raw procedure strings from a raw sourceCode string
     * @param srcCode raw sourceCode string, trimmed
     * @param procedures vector to store procedures as strings.
     * @return a list containing raw procedure strings, with keyword, with name, with brackets
     */
    static vector<string> extractProcedures(string srcCode, vector<string> procedures);

    /**
     * Extracts the name of a procedure.
     * @param procedure raw procedure string, inclusive of "procedure" keyword and brackets "{...}", trimmed
     * @return name of the procedure
     */
    static string extractProcName(string procedure);

    /**
     * Extracts statements from a raw string of statements
     * @param statements a raw string of statements (with semicolons)
     * @param statementList a predeclared vector of statements so that we can do recursion with it
     * @return a list of statements (without semicolons)
     */
    static vector<string> extractStatements(string statements, vector<string> statementList);

    /**
     * Parses an if or while statement string and extracts the conditional
     * expression as a string that sits between the outermost set of parentheses ( )
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
     * @param statement unprocessed statement string (without semicolon)
     * @return a Statement node
     */
    static shared_ptr<Statement> parseStatement(string statement);

    /**
     * Parses raw if-else block string
     * @param ifElseBlock if-else block string
     * @return an IfStatement node
     */
    static shared_ptr<IfStatement> parseIfElse(string ifElseBlock);

    /**
     * Parses raw while block string
     * @param whileBlock while block string
     * @return a WhileStatement node
     */
    static shared_ptr<WhileStatement> parseWhile(string whileBlock);

    /**
     * Parses raw conditional expression string
     * @param condExprStr raw conditional expression string
     * @return a ConditionalExpression node
     */
    static shared_ptr<ConditionalExpression> parseCondExpr(string condExprStr);

    /**
     * Parses a raw relational expression string
     * @param relExprStr raw relational expression string, no surrounding brackets
     * @return a RelationalExpression node
     */
    static shared_ptr<RelationalExpression> parseRelExpr(string relExprStr);

    /**
     * Checks if a statement is an assign statement
     * Serves as a helper function for parseStatement
     * @param statement raw statement to be checked (without semicolon)
     * @return true if statement is an assign statement, false otherwise
     */
    static bool isAssignStatement(string statement);
};

#endif // SPA_PARSER_H
