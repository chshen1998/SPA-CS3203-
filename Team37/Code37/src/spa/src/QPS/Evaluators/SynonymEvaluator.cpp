
using namespace std;

#include "SynonymEvaluator.h"
#include "../Structures/PqlQuery.h"
#include "PKB/QueryServicer.h"
#include "PKB/Types/StmtStmtRelationType.h"

vector<int> SynonymEvaluator::getAllLineNumOfStmtType(const StatementType ss)
{
    vector<int> result;

    for (shared_ptr<Statement> s : servicer->getAllStmt(ss)) {
        result.push_back(s->getLineNum());
    }

    return result;
}
