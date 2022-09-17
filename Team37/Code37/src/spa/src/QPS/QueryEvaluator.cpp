using namespace std;

#include <string>
#include <set>
#include <memory>

#include "QueryEvaluator.h"
#include "QPS.h"

#include "../AST/TNode.h"
#include "../PKB/PKB.h"
#include "../PKB/QueryServicer.h"


unordered_map<TokenType, StatementType> tokenTypeTostatementType = {
    { TokenType::STATEMENT, StatementType::STATEMENT},
    { TokenType::ASSIGN, StatementType::ASSIGN },
    { TokenType::CALL, StatementType::CALL },
    { TokenType::IF, StatementType::IF },
    { TokenType::PRINT, StatementType::PRINT },
    { TokenType::READ, StatementType::READ },
    { TokenType::WHILE, StatementType::WHILE }
};


QueryEvaluator::QueryEvaluator(PqlQuery& pqlQuery, shared_ptr<QueryServicer> s, list<string>& r):
    result(r), servicer(s), pq(pqlQuery) {}

void QueryEvaluator::evaluate() {
    const string selectSynonym = pq.select;
    const TokenType type = pq.declarations[selectSynonym];
    
    if (checkIfClauseExists() || checkIfSelectSynonymExistsInClause) {
        selectAll
    }


    if (type == TokenType::VARIABLE) {
        set<NameExpression> vars = servicer->getAllVar();

        for (auto v : vars) {
            result.push_back(v.getVarName());
        }
    }
}

void QueryEvaluator::selectAll(TokenType type) {
    if (type == TokenType::VARIABLE) {
        set<NameExpression> variables = servicer->getAllVar();

        for (NameExpression v : variables) {
            result.push_back(v.getVarName());
        }

    }

    else if (type == TokenType::CONSTANT) {
        set<ConstantExpression> constants = servicer->getAllConst();

        for (ConstantExpression c : constants) {
            result.push_back(to_string(c.getValue()));
        }
    }

    else {
        if (tokenTypeTostatementType.find(type) != tokenTypeTostatementType.end()) {
            StatementType stmtType = tokenTypeTostatementType[type];
            set<shared_ptr<Statement>> statements = servicer->getAllStmt(stmtType);
            
            for (shared_ptr<Statement> s : statements) {
                result.push_back(to_string(s->getLineNum()));
            }
        }
    }
}

inline bool QueryEvaluator::checkIfClauseExists() {
    return pq.patternClauses.empty() && pq.suchThatClauses.empty();
}

bool QueryEvaluator::checkIfSelectSynonymExistsInClause() {
    for (auto clause : pq.suchThatClauses) {
        if (pq.select == clause.left || pq.select == clause.right) {
            return true;
        }
    }

    for (auto clause : pq.patternClauses) {
        if (pq.select == clause.left || pq.select == clause.right) {
            return true;
        }
    }

    return false;
}