
using namespace std;

#include "../Structures/PqlQuery.h"
#include "PKB/Types/StmtStmtRelationType.h"
#include "PKB/QueryServicer.h"
#include "SuchThatEvaluator.h"

unordered_map<TokenType, StatementType> tokenTypeToStatementType = {
    { TokenType::STATEMENT, StatementType::STATEMENT},
    { TokenType::ASSIGN, StatementType::ASSIGN },
    { TokenType::CALL, StatementType::CALL },
    { TokenType::IF, StatementType::IF },
    { TokenType::PRINT, StatementType::PRINT },
    { TokenType::READ, StatementType::READ },
    { TokenType::WHILE, StatementType::WHILE }
};

void GeneralEvaluator::getAllLineNumOfStmtType(vector<int>& result, const StatementType ss) {
    for (shared_ptr<Statement> s : servicer->getAllStmt(ss)) {
        result.push_back(s->getLineNum());
    }
}
