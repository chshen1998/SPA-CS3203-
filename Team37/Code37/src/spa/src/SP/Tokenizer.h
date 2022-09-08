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

    static vector<shared_ptr<Procedure> > tokenizeProcedure(vector<string> names, vector<vector<string> > statements);

    static shared_ptr<ReadStatement> tokenizeRead(string line, int stmtNo, shared_ptr<TNode> parent);

    static shared_ptr<PrintStatement> tokenizePrint(string line, int stmtNo, shared_ptr<TNode> parent);

    static void tokenizeCall(string line);

    static void tokenizeCondition(string condition);

    static void tokenizeIf(string line, int stmtNo, shared_ptr<TNode> parent);

    static void tokenizeWhile(string line, int stmtNo, shared_ptr<TNode> parent);

    static void tokenizeAssignment(string line, int lineNo, shared_ptr<TNode> parent);

    static OperatedExpression tokenizeOperatedExpr(string line, vector<int> indexes);

    static void tokenizeOperatedExprByVar(string line, vector<int> indexes, string lhs, shared_ptr<TNode> parent, int lineNo);

    static vector<shared_ptr<Procedure> >
    tokenizeStatements(vector<shared_ptr<Procedure>> procedures, vector<vector<string>> statements);

    static shared_ptr<SourceCode>
    tokenize(shared_ptr<SourceCode> srcCode, vector<string> names, vector<vector<string>> statements);
};
