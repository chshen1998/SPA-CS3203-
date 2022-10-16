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

    vector<vector<string>> finalResult;
    
    for (Clause clause : pq.clauses) {
        if (clause.category == TokenType::WITH) {
            if (clause.leftAttr.type == TokenType::NONE) {
                falseBooleanClause = !withEvaluator.evaluateBooleanClause(clause);
            }
            else {
                finalResult = withEvaluator.evaluateClause(clause, finalResult);
            }
        }

        else if (clause.category == TokenType::PATTERN) {
            TokenType patternType = pq.declarations[clause.clauseType.value];
            if (patternType == TokenType::ASSIGN) {
                finalResult = assignEvaluator.evaluateClause(clause, finalResult);
            } else if (patternType == TokenType::WHILE) {
                finalResult = whileEvaluator.evaluateClause(clause, finalResult);
            } else if (patternType == TokenType::IF) {
                finalResult = ifEvaluator.evaluateClause(clause, finalResult);
            }
        }

        else {
            TokenType suchThatType = clause.clauseType.type;

            if (suchThatType == TokenType::CALLS) {
                if (clause.checkIfBooleanClause()) {
                    falseBooleanClause = !procProcEvaluator.evaluateBooleanClause(clause);
                }
                else {
                    finalResult = procProcEvaluator.evaluateSynonymClause(clause, finalResult);
                }
            }

            // Follows, Parents, Next, Affects
            else if (suchThatStmtRefStmtRef.find(suchThatType) != suchThatStmtRefStmtRef.end()) {

                if (clause.checkIfBooleanClause()) {
                    falseBooleanClause = !stmtStmtEvaluator.evaluateBooleanClause(clause);
                }
                else {
                    finalResult = stmtStmtEvaluator.evaluateSynonymClause(clause, finalResult);
                }
            }

            // Uses_P, Modifies_P, Calls
            else if (clause.left.type == TokenType::STRING || pq.declarations[clause.left.value] == TokenType::PROCEDURE) {

                if (clause.checkIfBooleanClause()) {
                    falseBooleanClause = !procVarEvaluator.evaluateBooleanClause(clause);
                }
                else {
                    finalResult = procVarEvaluator.evaluateSynonymClause(clause, finalResult);
                }
            }
            // Uses_S, Modifies_S
            else  {
                if (clause.checkIfBooleanClause()) {
                    falseBooleanClause = !stmtVarEvaluator.evaluateBooleanClause(clause);
                }
                else {
                    finalResult = stmtVarEvaluator.evaluateSynonymClause(clause, finalResult);
                }
            }
        }

        if (falseBooleanClause) {
            break;
        }
    }

    if (isResultBoolean) {
        if (falseBooleanClause || finalResult.size() == 1) {
            result.push_back("FALSE");
        }
        else {
            result.push_back("TRUE");
        }
        return;
    }

    if (falseBooleanClause) {
        return;
    }
   
    FinalEvaluator finalEvaluator = FinalEvaluator(servicer, pq.declarations, pq);
    finalEvaluator.getFinalResult(result, finalResult);
}
