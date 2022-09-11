#include <string>
#include <vector>

using namespace std;

#include "AST/SourceCode.h"
#include "AST/Procedure.h"
#include "AST/Statement/ReadStatement.h"
#include "AST/Statement/PrintStatement.h"
#include "AST/Statement/AssignStatement.h"
#include "AST/Statement/IfStatement.h"
#include "AST/Expression/RelationalFactor/OperatedExpression.h"
#include "AST/Expression/RelationalFactor/ConstantExpression.h"
#include "AST/Expression/RelationalFactor/NameExpression.h"

class Tokenizer {
public:
    Tokenizer();

    /**
     * Creates ReadStatement object with specified parent and stmtNo.
     * @param line read statement in string
     * @param stmtNo statement number of statement
     * @param parent parent node of read statement
     * @return ReadStatement object
     */
    static shared_ptr<ReadStatement> tokenizeRead(string line, int stmtNo, shared_ptr<TNode> parent);

    /**
     * Creates PrintStatement object with specified parent and stmtNo.
     * @param line print statement in string
     * @param stmtNo statement number of statement
     * @param parent parent node of read statement
     * @return PrintStatement object
     */
    static shared_ptr<PrintStatement> tokenizePrint(string line, int stmtNo, shared_ptr<TNode> parent);


    /**
     * Creates statement objects and sets procedure as the parent node
     * @param procedures vector containing all procedures of program
     * @param statements vector containing statement lists as strings for each procedure
     * @return vector containing procedures with corresponding statement objects
     */
    static vector<shared_ptr<Procedure>>
    tokenizeStatements(vector<shared_ptr<Procedure>> procedures, vector<vector<string>> statements);

    /**
     * Converts procedures as strings to Procedure objects.
     * @param names vector containing all names of procedures in strings
     * @param statements vector containing vector of statements corresponding to each procedure
     * @return vector containing procedures containing statement lists.
     */
    static vector<shared_ptr<Procedure>> tokenizeProcedure(vector<string> names, vector<vector<string>> statements);

    /**
     * Tokenizes SourceCode and sets SourceCode as parent to all corresponding procedures.
     * @param srcCode sourcecode object that acts as root node
     * @param names vector of all procedure names of program
     * @param statements vector of vector containing statements as strings for each corresponding procedure
     * @return sourcecode node that now has all procedures in its procedure list
     */
    static shared_ptr<SourceCode>
    tokenize(shared_ptr<SourceCode> srcCode, vector<string> names, vector<vector<string>> statements);

//    static void tokenizeOperatedExprByVar(string line, vector<int> indexes, string lhs, shared_ptr<TNode> parent, int lineNo);

//    static void tokenizeCall(string line);
//
//    static void tokenizeCondition(string condition);
//
//    static void tokenizeIf(string line, int stmtNo, shared_ptr<TNode> parent);
//
//    static void tokenizeWhile(string line, int stmtNo, shared_ptr<TNode> parent);

//    static void tokenizeAssignment(string line, int lineNo, shared_ptr<TNode> parent);
};
