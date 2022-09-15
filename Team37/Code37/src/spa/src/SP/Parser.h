#ifndef SPA_PARSER_H
#define SPA_PARSER_H

using namespace std;

#include <string>
#include <vector>

#include "AST/SourceCode.h"
#include "AST/Procedure.h"
#include "AST/Expression/ConditionalExpression/ConditionalExpression.h"

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
     * Extracts statements of a procedure as strings.
     * @param procedure
     * @return StatementList which is a vector containing statements as strings.
     */
    static vector<string> extractStatements(string procedure);

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
     * @param ifElse if-else block string
     * @return an IfStatement node
     */
    static shared_ptr<IfStatement> parseIfElse(string ifElse);

    /**
     * Parses unprocessed while block string and produces a statement list
     * which is a vector containing unprocessed statements as strings.
     * @param whileBlock while block string
     * @param stmtNo statement number of while statement
     * @param parent parent node of the while statement
     * @return a WhileStatement node
     */
    static shared_ptr<WhileStatement> parseWhile(string whileBlock, int stmtNo, shared_ptr<TNode> parent);

    /**
     * Parses unprocessed conditional expression string
     * @param condExprStr conditional expression string
     * @return a ConditionalExpression node
     */
    static shared_ptr<ConditionalExpression> parseCondExpr(string condExprStr);
};


#endif //SPA_PARSER_H
