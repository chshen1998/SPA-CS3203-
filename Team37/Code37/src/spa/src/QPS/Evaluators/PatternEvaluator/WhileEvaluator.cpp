using namespace std;

#include "WhileEvaluator.h"
#include "../EvaluatorUtils.h"
#include "QPS/Structures/PqlQuery.h"
#include "QPS/Structures/PqlToken.h"
#include "QPS/Types/TokenType.h"

using namespace EvaluatorUtils;

vector<vector<string>> WhileEvaluator::evaluateSynonymClause(shared_ptr<Clause> clause, vector<vector<string>> intermediate) {
    PqlToken leftArg = clause->left;
    TokenType patternType = declarations[clause->clauseType.value];
    StatementType patternStmtType = tokenTypeToStatementType[patternType];
    vector<vector<string>> finalTable;
    vector<int> allWhileStmtLines = getAllLineNumOfStmtType(patternStmtType);

    // Add in the column header
    finalTable.push_back(vector<string>{clause->clauseType.value});

    vector<int> finalResult;

    if (leftArg.type == TokenType::SYNONYM) {
        // First argument is variable synonym --> Eg. w (v,_)
        // Add synonym column header
        finalTable[0].push_back(leftArg.value);
        for (int line: allWhileStmtLines) {
            for (string v: servicer->forwardRetrieveRelation(line, StmtVarRelationType::USESSVPREDICATE)) {
                finalTable.push_back(vector<string>{to_string(line), v});
            }
        }
    } else if (leftArg.type == TokenType::STRING) {
        // First argument is variable name in quotes --> Eg. w ("x",_)
        vector<int> allStmtsWithLeftArg = servicer->reverseRetrieveRelation(leftArg.value, StmtVarRelationType::USESSVPREDICATE);
        getLineNumInteresection(finalResult, allStmtsWithLeftArg, allWhileStmtLines);
        for (int line: finalResult) {
            finalTable.push_back(vector<string>{to_string(line)});
        }
    } else {
        // First argument is wildcard --> Eg. w (_,_)
        for (int line: allWhileStmtLines) {
            if (!servicer->forwardRetrieveRelation(line, StmtVarRelationType::USESSVPREDICATE).empty()) {
                finalTable.push_back(vector<string>{to_string(line)});
            }
        }
    }
    return JoinTable(finalTable, intermediate);
}
