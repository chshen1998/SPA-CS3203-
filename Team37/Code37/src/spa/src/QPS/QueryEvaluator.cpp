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
#include "QPS/Evaluators/SuchThatEvaluator/ProcProcEvaluator.h"
#include "QPS/Evaluators/PatternEvaluator/AssignEvaluator.h"
#include "QPS/Evaluators/PatternEvaluator/WhileEvaluator.h"
#include "QPS/Evaluators/PatternEvaluator/IfEvaluator.h"
#include "QPS/Evaluators/FilterEvaluator/WithEvaluator.h"
#include "QPS/Evaluators/FilterEvaluator/FinalEvaluator.h"
#include "QPS/Evaluators/EvaluatorUtils.h"

#include "AST/Expression/RelationalFactor/NameExpression.h"
#include "AST/Expression/RelationalFactor/ConstantExpression.h"
#include "AST/Procedure.h"
#include "AST/Statement/Statement.h"

#include "PKB/QueryServicer.h"


QueryEvaluator::QueryEvaluator(PqlQuery& pqlQuery, shared_ptr<QueryServicer> s, list<string>& r) :
    result(r), servicer(s), pq(pqlQuery) {}

void QueryEvaluator::evaluate() {
    const bool isResultBoolean = pq.selectObjects[0].type == SelectType::BOOLEAN;
    bool falseBooleanClause = false;

    // TODO: restructure this 
    StmtStmtEvaluator stmtStmtEvaluator = StmtStmtEvaluator(servicer, pq.declarations);
    StmtVarEvaluator stmtVarEvaluator = StmtVarEvaluator(servicer, pq.declarations);
    ProcVarEvaluator procVarEvaluator = ProcVarEvaluator(servicer, pq.declarations);
    ProcProcEvaluator procProcEvaluator = ProcProcEvaluator(servicer, pq.declarations);
    AssignEvaluator assignEvaluator = AssignEvaluator(servicer, pq.declarations);
    WithEvaluator withEvaluator = WithEvaluator(servicer, pq.declarations);
    WhileEvaluator whileEvaluator = WhileEvaluator(servicer, pq.declarations);
    IfEvaluator ifEvaluator = IfEvaluator(servicer, pq.declarations);

    // Solve the boolean clauses
    for (Clause booleanClause : pq.booleanClauses) {
        if (booleanClause.category == TokenType::WITH) {
            falseBooleanClause = !withEvaluator.evaluateBooleanClause(booleanClause);
        }
        else {
            TokenType suchThatType = booleanClause.clauseType.type;

            if (suchThatType == TokenType::CALLS || suchThatType == TokenType::CALLS_A) {
                falseBooleanClause = !procProcEvaluator.evaluateBooleanClause(booleanClause);
            }

            else if (suchThatStmtRefStmtRef.find(suchThatType) != suchThatStmtRefStmtRef.end()) {
                falseBooleanClause = !stmtStmtEvaluator.evaluateBooleanClause(booleanClause);
            }

            // Uses_P, Modifies_P
            else if (booleanClause.left.type == TokenType::STRING || pq.declarations[booleanClause.left.value] == TokenType::PROCEDURE) {
                falseBooleanClause = !procVarEvaluator.evaluateBooleanClause(booleanClause);
            }

            // Uses_S, Modifies_S
            else {
                falseBooleanClause = !stmtVarEvaluator.evaluateBooleanClause(booleanClause);
            }
        }

        if (falseBooleanClause) {
            if (isResultBoolean) {
                result.push_back("FALSE");
            }
            return;
        }
    }

    vector<vector<vector<string>>> listOfIntermediateTables;
    listOfIntermediateTables.reserve(pq.clauses.size());

    // Solve the Synonym Clauses
    for (vector<Clause> clauseGroup : pq.clauses) {
        vector<vector<string>> intermediateTable;

        for (Clause clause : clauseGroup) {
            if (clause.category == TokenType::WITH) {
                intermediateTable = withEvaluator.evaluateClause(clause, intermediateTable);
            }
            else if (clause.category == TokenType::PATTERN) {
                TokenType patternType = pq.declarations[clause.clauseType.value];

                if (patternType == TokenType::ASSIGN) {
                    intermediateTable = assignEvaluator.evaluateClause(clause, intermediateTable);
                }
                else if (patternType == TokenType::WHILE) {
                    intermediateTable = whileEvaluator.evaluateClause(clause, intermediateTable);
                }
                else if (patternType == TokenType::IF) {
                    intermediateTable = ifEvaluator.evaluateClause(clause, intermediateTable);
                }
            }

            else {
                TokenType suchThatType = clause.clauseType.type;

                if (suchThatType == TokenType::CALLS || suchThatType == TokenType::CALLS_A) {
                    intermediateTable = procProcEvaluator.evaluateSynonymClause(clause, intermediateTable);
                }

                // Follows, Parents, Next, Affects
                else if (suchThatStmtRefStmtRef.find(suchThatType) != suchThatStmtRefStmtRef.end()) {
                    intermediateTable = stmtStmtEvaluator.evaluateSynonymClause(clause, intermediateTable);
                }

                // Uses_P, Modifies_P
                else if (clause.left.type == TokenType::STRING || pq.declarations[clause.left.value] == TokenType::PROCEDURE) {
                    intermediateTable = procVarEvaluator.evaluateSynonymClause(clause, intermediateTable);
                }
                // Uses_S, Modifies_S
                else {
                    intermediateTable = stmtVarEvaluator.evaluateSynonymClause(clause, intermediateTable);
                }
            }

            // Terminate early if any clause is invalid or empty
            if (intermediateTable.size() <= 1) {
                if (isResultBoolean) {
                    result.push_back("FALSE");
                }
                return;
            }
        }

        listOfIntermediateTables.push_back(intermediateTable);
    }

    vector<vector<string>> finalResult;
    // Combine the smaller intermediate tables
    for (vector<vector<string>> table : listOfIntermediateTables) {
        finalResult = EvaluatorUtils::JoinTable(finalResult, table);

        if (finalResult.size() <= 1) {
            if (isResultBoolean) {
                result.push_back("FALSE");
            }  
            return;
        }
    }

    if (isResultBoolean) {
        result.push_back("TRUE");
        return;
    }

    FinalEvaluator finalEvaluator = FinalEvaluator(servicer, pq.declarations, pq);
    finalEvaluator.getFinalResult(result, finalResult);
}



