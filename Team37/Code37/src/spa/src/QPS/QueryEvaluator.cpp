using namespace std;

#include <string>
#include <set>
#include <memory>

#include "QueryEvaluator.h"
#include "QPS.h"

#include "../AST/TNode.h"
#include "../PKB/PKB.h"
#include "../PKB/QueryServicer.h"


QueryEvaluator::QueryEvaluator(PqlQuery pqlQuery, shared_ptr<QueryServicer> s, list<string>& r):
    result(r), servicer(s)
{
    pq = pqlQuery;
}

void QueryEvaluator::evaluate() {
    const string selectSynonym = pq.select;
    const TokenType type = pq.declarations[selectSynonym];
    
    if (type == TokenType::VARIABLE) {
        set<NameExpression> vars = servicer->getAllVar();

        for (auto v : vars) {
            result.push_back(v.getVarName());
        }
    }
}
