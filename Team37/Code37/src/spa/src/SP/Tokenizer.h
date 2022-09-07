#include <string>
#include <vector>

using namespace std;

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

    static shared_ptr<TNode> tokenize(vector<string> lines);
    static shared_ptr<Procedure> tokenizeProcedure(string line, int startIdx, string keyword);
    static shared_ptr<Procedure> tokenizeProcedure(string line);
    static void tokenizeRead(string line, int stmtNo, shared_ptr<TNode> parent);
    static void tokenizePrint(string line, int stmtNo, shared_ptr<TNode> parent);
    static void tokenizeCall(string line);
    static void tokenizeCondition(string condition);
    static void tokenizeIf(string line, int stmtNo, shared_ptr<TNode> parent);
    static void tokenizeWhile(string line, int stmtNo, shared_ptr<TNode> parent);
    static void tokenizeAssignment(string line, int lineNo, shared_ptr<TNode> parent);
    static OperatedExpression tokenizeOperatedExpr(string line, vector<int> indexes);
    static void tokenizeOperatedExprByVar(string line, vector<int> indexes, string lhs, shared_ptr<TNode> parent, int lineNo);
};
