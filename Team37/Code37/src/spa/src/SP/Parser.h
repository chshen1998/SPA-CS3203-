#ifndef SPA_PARSER_H
#define SPA_PARSER_H

using namespace std;

#include <string>
#include <vector>

#include "AST/SourceCode.h"
#include "AST/Procedure.h"

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
     * @param procedureNode parent Procedure node of the statement
     * @return a Statement node
     */
    static shared_ptr<Statement> parseStatement(string statement, shared_ptr<Procedure> procedureNode);

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

};


#endif //SPA_PARSER_H
