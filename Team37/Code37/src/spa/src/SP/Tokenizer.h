#include <string>
#include <vector>

using namespace std;

#include "AST/Procedure.h"
#include "AST/Statement/ReadStatement.h"
#include "AST/Statement/PrintStatement.h"
#include "AST/Statement/AssignStatement.h"
#include "AST/Expression/OperatedExpression.h"
#include "AST/Expression/ConstantExpression.h"
#include "AST/Expression/NameExpression.h"

class Tokenizer {
public:
    Tokenizer();

    static void tokenize(vector<string> lines);
    static shared_ptr<Procedure> tokenizeProcedure(string line, int startIdx, string keyword);
    static shared_ptr<Procedure> tokenizeProcedure(string line);
    static void tokenizeRead(string line, int stmtNo, shared_ptr<TNode> parent);
    static void tokenizePrint(string line, int stmtNo, shared_ptr<TNode> parent);
    static void tokenizeCall(string line);
    static void tokenizeCondition(string condition);
    static void tokenizeIf(string line);
    static void tokenizeWhile(string line);
    static void tokenizeAssignment(string line, int lineNo, shared_ptr<TNode> parent);
    static OperatedExpression tokenizeOperatedExpr(string line, vector<int> indexes);
};
