using namespace std;

#include <string>
#include <set>
#include <memory>
#include <unordered_map>

#include "QueryEvaluator.h"
#include "./Structures/PqlError.h"
#include "./Structures/PqlToken.h"
#include "./Structures/PqlQuery.h"
#include "./Types/ErrorType.h"
#include "./Types/TokenType.h"

#include "QPS/Evaluators/SuchThatEvaluator/StmtStmtEvaluator.h"
#include "QPS/Evaluators/SuchThatEvaluator/StmtVarEvaluator.h"
#include "QPS/Evaluators/SuchThatEvaluator/ProcVarEvaluator.h"
#include "QPS/Evaluators/PatternEvaluator/AssignEvaluator.h"
#include "QPS/Evaluators/WithEvaluator.h"

#include "AST/Expression/RelationalFactor/NameExpression.h"
#include "AST/Expression/RelationalFactor/ConstantExpression.h"
#include "AST/Procedure.h"
#include "AST/Statement/Statement.h"

#include "PKB/QueryServicer.h"


QueryEvaluator::QueryEvaluator(PqlQuery& pqlQuery, shared_ptr<QueryServicer> s, list<string>& r) :
    result(r), servicer(s), pq(pqlQuery) {}

void QueryEvaluator::evaluate() {
    const string selectSynonym = pq.select;
    const TokenType type = pq.declarations[selectSynonym];
 
    // TODO: restructure this 
    StmtStmtEvaluator stmtStmtEvaluator = StmtStmtEvaluator(servicer, pq.declarations);
    StmtVarEvaluator stmtVarEvaluator = StmtVarEvaluator(servicer, pq.declarations);
    ProcVarEvaluator procVarEvaluator = ProcVarEvaluator(servicer, pq.declarations);
    AssignEvaluator assignEvaluator = AssignEvaluator(servicer, pq.declarations);
    WithEvaluator withEvaluator = WithEvaluator(servicer, pq.declarations);

    vector<vector<string>> finalResult;
    
    for (Clause clause : pq.clauses) {
        if (clause.category == TokenType::WITH) {
            if (clause.leftAttr.type == TokenType::NONE && clause.rightAttr.type == TokenType::NONE) {
                if (!withEvaluator.evaluateBooleanClause(clause)) {
                    return;
                }
            }
            else {
                finalResult = withEvaluator.evaluateClause(clause, finalResult);
            }
        }

        else if (clause.category == TokenType::PATTERN) {
            finalResult = assignEvaluator.evaluateClause(clause, finalResult);

            for (vector<string> s : finalResult) {
                for (string s1 : s) {
                    cout << s1 << endl;
                }
            }
        }

        else {
            TokenType suchThatType = clause.clauseType.type;

            // Follows, Parents, Next, Affects
            if (suchThatStmtRefStmtRef.find(suchThatType) != suchThatStmtRefStmtRef.end()) {

                if (clause.checkIfBooleanClause()) {
                    if (!stmtStmtEvaluator.evaluateBooleanClause(clause)) {
                        return;
                    }
                }
                else {
                    finalResult = stmtStmtEvaluator.evaluateSynonymClause(clause, finalResult);
                }
            }

            // Uses_S, Modifies_S
            else if (suchThatStmtRefVarRef.find(suchThatType) != suchThatStmtRefVarRef.end()) {
                if (clause.checkIfBooleanClause()) {
                    if (!stmtVarEvaluator.evaluateBooleanClause(clause)) {
                        return;
                    }
                }
                else {
                    finalResult = stmtVarEvaluator.evaluateSynonymClause(clause, finalResult);
                }
            }

            // Uses_P, Modifies_P, Calls
            else {
                if (clause.checkIfBooleanClause()) {
                    if (!procVarEvaluator.evaluateBooleanClause(clause)) {
                        return;
                    }
                }
                else {
                    finalResult = procVarEvaluator.evaluateSynonymClause(clause, finalResult);
                }
            }
        }
    }

    // If there are no clauses
    if (finalResult.size() == 0) {
        selectAll(type);
        return;
    }


    getResultFromFinalTable(finalResult);
}


void QueryEvaluator::getResultFromFinalTable(const vector<vector<string>>& table) {
    int index = 0;
    // Find the column index where the synonym value == select synonym value
    for (int i = 0; i < table[0].size(); i++) {
        if (table[0][i] == pq.select) {
            index = i;
            break;
        }
    }

    // Add the result values of that column into result
    for (int j = 1; j < table.size(); j++) {
        if (find(result.begin(), result.end(), table[j][index]) == result.end()) {
            cout << "Final table combination" << endl;
            cout << table[j][index] << endl;
            result.push_back(table[j][index]);
        }
    }

    return;
}


void QueryEvaluator::selectAll(TokenType type) {
    if (type == TokenType::VARIABLE) {
        for (NameExpression v : servicer->getAllVar()) {
            result.push_back(v.getVarName());
        }
    }

    else if (type == TokenType::CONSTANT) {
        for (ConstantExpression c : servicer->getAllConst()) {
            result.push_back(to_string(c.getValue()));
        }
    }

    else if (type == TokenType::PROCEDURE) {
        for (Procedure p : servicer->getAllProc()) {
            result.push_back(p.getProcedureName());
        }
    }

    else {
        if (tokenTypeToStatementType.find(type) != tokenTypeToStatementType.end()) {
            StatementType stmtType = tokenTypeToStatementType[type];
            set<shared_ptr<Statement>> statements = servicer->getAllStmt(stmtType);
            
            for (shared_ptr<Statement> s : statements) {
                result.push_back(to_string(s->getLineNum()));
            }
        }
    }
}

