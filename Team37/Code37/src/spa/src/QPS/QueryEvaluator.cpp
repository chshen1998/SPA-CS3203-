using namespace std;

#include <string>
#include <set>
#include <memory>

#include "QueryEvaluator.h"
#include "QPS.h"

#include "../AST/TNode.h"
#include "../PKB/PKB.h"
#include "../PKB/QueryServicer.h"


QueryEvaluator::QueryEvaluator(PqlQuery pqlQuery, shared_ptr<QueryServicer> s, list<string>& r) {
    pq = pqlQuery;
    servicer = s;
    result = r;
}

void QueryEvaluator::evaluate() {
    QuerySelect();
}

void QueryEvaluator::QuerySelect() {
    const string selectSynonym = pq.select;
    const TokenType type = pq.declarations[selectSynonym];
    set<string> setOfStrings;

    if (type == TokenType::VARIABLE) {
        set<NameExpression> vars = servicer->getAllVar();
        
        for (auto v : vars) {
            result.push_back(v.getVarName());
        }
    }
    
    else if (type == TokenType::CONSTANT) {
        set<ConstantExpression> consts = servicer->getAllConst();

        for (auto c : consts) {
            setOfStrings.insert(to_string(c.getValue()));
        }
    }

    else {}
   
}




