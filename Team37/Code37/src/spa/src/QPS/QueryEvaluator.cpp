using namespace std;

#include <string>
#include <set>
#include <memory>

#include "QueryEvaluator.h"
#include "QPS.h"

#include "../AST/TNode.h"
#include "../PKB/PKB.h"
#include "../PKB/QueryServicer.h"
#include "../PKB/elementType.h"


QueryEvaluator::QueryEvaluator(PqlQuery pqlQuery, shared_ptr<QueryServicer> s) {
    pq = pqlQuery;
    servicer = s;
}

set<string> QueryEvaluator::CallPKB() {
    QuerySelect();

    evaluateSuchThatClause();
    evaluatePatternClause();

    return selectResult;
}

void QueryEvaluator::QuerySelect() {
    const string selectSynonym = pq.select;
    const TokenType type = pq.declarations[selectSynonym];
    ElementType element;

    switch (type) {
    case TokenType::VARIABLE:
        element = ElementType::VARIABLE;
    case TokenType::CONSTANT:
        element = ElementType::CONSTANT;
    default:
        throw "Token does not exists";
    }

    set<shared_ptr<TNode>> result = servicer->retrieveAll(element);

}


void QueryEvaluator::evaluateSuchThatClause() {
    if (pq.suchThatClause) {

    }


}

void QueryEvaluator::evaluatePatternClause() {
    if (pq.patternClause)


}


