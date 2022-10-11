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
#include "QPS/Evaluators/EvaluatorUtils.h"
#include "QPS/Evaluators/WithEvaluator.h"

#include "AST/Expression/RelationalFactor/NameExpression.h"
#include "AST/Expression/RelationalFactor/ConstantExpression.h"
#include "AST/Procedure.h"
#include "AST/Statement/Statement.h"

#include "PKB/QueryServicer.h"


QueryEvaluator::QueryEvaluator(PqlQuery& pqlQuery, shared_ptr<QueryServicer> s, list<string>& r) :
    result(r), servicer(s), pq(pqlQuery) {}

void QueryEvaluator::evaluate() {
    // Hack fix: must urgently fix this in next PR
    const string selectSynonym = pq.selectObjects[0].synonym;
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
            if (clause.leftAttr.type == TokenType::NONE) {
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

            // Uses_P, Modifies_P, Calls
            else if (clause.left.type == TokenType::STRING || pq.declarations[clause.left.value] == TokenType::PROCEDURE) {

                if (clause.checkIfBooleanClause()) {
                    if (!procVarEvaluator.evaluateBooleanClause(clause)) {
                        return;
                    }
                }
                else {
                    finalResult = procVarEvaluator.evaluateSynonymClause(clause, finalResult);
                }
            }
            // Uses_S, Modifies_S
            else  {
                if (clause.checkIfBooleanClause()) {
                    if (!stmtVarEvaluator.evaluateBooleanClause(clause)) {
                        return;
                    }
                }
                else {
                    finalResult = stmtVarEvaluator.evaluateSynonymClause(clause, finalResult);
                }
            }
        }
    }

    // If there are no clauses (our table will not even have column headers)
    if (finalResult.size() == 0) {
        getAllCombinations();
        return;
    }

    getResultFromFinalTable(finalResult);
}


void QueryEvaluator::getResultFromFinalTable(const vector<vector<string>>& table) {
    int index = 0;
    // Find the column index where the synonym value == select synonym value
    // TODO: Update to get tuples of elems and logic for attrRef
    // - Tuples just get indexes of elements we need
    // - Apart from call, print and read, any attrRef returns the default value
    for (int i = 0; i < table[0].size(); i++) {
        // TODO: update for attrRef
        if (table[0][i] == pq.selectObjects[0].synonym) {
            index = i;
            break;
        }
    }

    // Add the result values of that column into result
    for (int j = 1; j < table.size(); j++) {
        if (find(result.begin(), result.end(), table[j][index]) == result.end()) {
            result.push_back(table[j][index]);
        }
    }

    return;
}

// In the case where we have no clauses, we get all combinations for tuples
void QueryEvaluator::getAllCombinations() {
    if (pq.selectObjects[0].type == SelectType::BOOLEAN) {
        result.push_back("TRUE");
        return;
    }
    
    vector<vector<string>> finalResult;

    for (SelectObject s : pq.selectObjects) {
        TokenType declarationType = pq.declarations[s.synonym];

        if (s.type == SelectType::ATTRNAME) {            
            switch (s.attrName.type) {
            case TokenType::VARNAME:
                declarationType = TokenType::VARIABLE;
                break;
            case TokenType::PROCNAME:
                declarationType = TokenType::PROCEDURE;
                break;
            case TokenType::VALUE:
                declarationType = TokenType::CONSTANT;
                break;
            } 
        }

        finalResult.push_back(selectAll(declarationType));
    }

    vector<int> indexes;
    indexes.reserve(finalResult.size());

    for (int i = 0; i < finalResult.size(); i++) {
        indexes.push_back(0);
    }

    while (true) {
        // We add combine our row into a string, and then add it to result
        string row = "";
        for (int k = 0; k < finalResult.size(); k++) {
            row += finalResult[k][indexes[k]];

            if (k != finalResult.size() - 1) {
                row += " ";
            }
        }
        result.push_back(row);

        int current = 0;
        indexes[current] += 1;

        while (indexes[current] == finalResult[current].size()) {
            if (current == finalResult.size() - 1) {
                return;
            }

            indexes[current] = 0;
            current += 1;
            indexes[current] += 1;
        }
    }
}



vector<string> QueryEvaluator::selectAll(TokenType type) {
    vector<string> result;

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

    return result;
}