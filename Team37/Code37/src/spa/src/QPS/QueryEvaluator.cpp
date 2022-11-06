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

#include "QPS/Evaluators/SuchThatEvaluator/StmtStmtEvaluator/StmtStmtBooleanEvaluator.h"
#include "QPS/Evaluators/SuchThatEvaluator/StmtStmtEvaluator/StmtStmtSynonymEvaluator.h"
#include "QPS/Evaluators/SuchThatEvaluator/StmtVarEvaluator/StmtVarBooleanEvaluator.h"
#include "QPS/Evaluators/SuchThatEvaluator/StmtVarEvaluator/StmtVarSynonymEvaluator.h"
#include "QPS/Evaluators/SuchThatEvaluator/ProcVarEvaluator/ProcVarBooleanEvaluator.h"
#include "QPS/Evaluators/SuchThatEvaluator/ProcVarEvaluator/ProcVarSynonymEvaluator.h"
#include "QPS/Evaluators/SuchThatEvaluator/ProcProcEvaluator/ProcProcBooleanEvaluator.h"
#include "QPS/Evaluators/SuchThatEvaluator/ProcProcEvaluator/ProcProcSynonymEvaluator.h"

#include "QPS/Evaluators/PatternEvaluator/AssignEvaluator.h"
#include "QPS/Evaluators/PatternEvaluator/WhileEvaluator.h"
#include "QPS/Evaluators/PatternEvaluator/IfEvaluator.h"

#include "QPS/Evaluators/FilterEvaluator/WithBooleanEvaluator.h"
#include "QPS/Evaluators/FilterEvaluator/WithSynonymEvaluator.h"
#include "QPS/Evaluators/FilterEvaluator/FinalEvaluator.h"

#include "QPS/Evaluators/EvaluatorUtils.h"
#include "QPS/Evaluators/BooleanEvaluator.h"
#include "QPS/Evaluators/SynonymEvaluator.h"

#include "AST/Expression/RelationalFactor/NameExpression.h"
#include "AST/Expression/RelationalFactor/ConstantExpression.h"
#include "AST/Procedure.h"
#include "AST/Statement/Statement.h"

#include "PKB/QueryServicer.h"


QueryEvaluator::QueryEvaluator(shared_ptr<PqlQuery> pqlQuery, shared_ptr<QueryServicer> s, list<string>& r) :
    result(r), servicer(s), pq(pqlQuery) {}

void QueryEvaluator::evaluate() {
    const bool isResultBoolean = pq->selectObjects[0]->type == SelectType::BOOLEAN;

    unique_ptr<BooleanEvaluator> booleanEvaluator;

    // Solve the boolean clauses
    for (shared_ptr<Clause> booleanClause : pq->booleanClauses) {
        if (booleanClause->category == TokenType::WITH) {
            booleanEvaluator = make_unique<WithBooleanEvaluator>(WithBooleanEvaluator(servicer, pq->declarations));
        }
        else {
            TokenType suchThatType = booleanClause->clauseType.type;

            if (suchThatType == TokenType::CALLS || suchThatType == TokenType::CALLS_A) {
                booleanEvaluator = make_unique<ProcProcBooleanEvaluator>(ProcProcBooleanEvaluator(servicer, pq->declarations));
            }

            else if (suchThatStmtRefStmtRef.find(suchThatType) != suchThatStmtRefStmtRef.end()) {
                booleanEvaluator = make_unique<StmtStmtBooleanEvaluator>(StmtStmtBooleanEvaluator(servicer, pq->declarations));
            }

            // Uses_P, Modifies_P
            else if (booleanClause->left.type == TokenType::STRING || pq->declarations[booleanClause->left.value] == TokenType::PROCEDURE) {
                booleanEvaluator = make_unique<ProcVarBooleanEvaluator>(ProcVarBooleanEvaluator(servicer, pq->declarations));
            }

            // Uses_S, Modifies_S
            else {
                booleanEvaluator = make_unique<StmtVarBooleanEvaluator>(StmtVarBooleanEvaluator(servicer, pq->declarations));
            }
        }

        if (!booleanEvaluator->evaluateBooleanClause(booleanClause)) {
            if (isResultBoolean) {
                result.push_back("FALSE");
            }
            return;
        }
    }

    vector<vector<vector<string>>> listOfIntermediateTables;
    listOfIntermediateTables.reserve(pq->clauses.size());
    unique_ptr<SynonymEvaluator> synonymEvaluator;

    // Solve the Synonym Clauses
    for (vector<shared_ptr<Clause>> clauseGroup : pq->clauses) {
        vector<vector<string>> intermediateTable;

        for (shared_ptr<Clause> clause : clauseGroup) {
            if (clause->category == TokenType::WITH) {
                synonymEvaluator = make_unique<WithSynonymEvaluator>(WithSynonymEvaluator(servicer, pq->declarations));
            }
            else if (clause->category == TokenType::PATTERN) {
                TokenType patternType = pq->declarations[clause->clauseType.value];

                if (patternType == TokenType::ASSIGN) {
                    synonymEvaluator = make_unique<AssignEvaluator>(AssignEvaluator(servicer, pq->declarations));
                }
                else if (patternType == TokenType::WHILE) {
                    synonymEvaluator = make_unique<WhileEvaluator>(WhileEvaluator(servicer, pq->declarations));
                }
                else if (patternType == TokenType::IF) {
                    synonymEvaluator = make_unique<IfEvaluator>(IfEvaluator(servicer, pq->declarations));
                }
            }
            else {
                TokenType suchThatType = clause->clauseType.type;

                if (suchThatType == TokenType::CALLS || suchThatType == TokenType::CALLS_A) {
                    synonymEvaluator = make_unique<ProcProcSynonymEvaluator>(ProcProcSynonymEvaluator(servicer, pq->declarations));
                }

                else if (suchThatStmtRefStmtRef.find(suchThatType) != suchThatStmtRefStmtRef.end()) {
                    synonymEvaluator = make_unique<StmtStmtSynonymEvaluator>(StmtStmtSynonymEvaluator(servicer, pq->declarations));
                }

                // Uses_P, Modifies_P
                else if (clause->left.type == TokenType::STRING || pq->declarations[clause->left.value] == TokenType::PROCEDURE) {
                    synonymEvaluator = make_unique<ProcVarSynonymEvaluator>(ProcVarSynonymEvaluator(servicer, pq->declarations));
                }

                // Uses_S, Modifies_S
                else {
                    synonymEvaluator = make_unique<StmtVarSynonymEvaluator>(StmtVarSynonymEvaluator(servicer, pq->declarations));
                }
            }

            intermediateTable = synonymEvaluator->evaluateSynonymClause(clause, intermediateTable);
            
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

    FinalEvaluator finalEvaluator = FinalEvaluator(servicer, pq->declarations, pq);
    finalEvaluator.getFinalResult(result, finalResult);
}



