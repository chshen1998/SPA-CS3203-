using namespace std;

#include "FilterEvaluator.h"
#include "PKB/Types/StatementType.h"
#include "PKB/Types/StmtVarRelationType.h"
#include "QPS/Evaluators/EvaluatorUtils.h"
#include "QPS/Structures/PqlQuery.h"
#include "QPS/Structures/PqlToken.h"
#include "QPS/Types/TokenType.h"

using namespace EvaluatorUtils;

string FilterEvaluator::updatedColumnName(const PqlToken& token)
{
    return token.type == TokenType::CALL ? token.value + ".procName" : token.value + ".varName";
}

bool FilterEvaluator::addAttrName(vector<vector<string>>& intermediate, const PqlToken& token)
{
    if (doubleAttrTokens.find(declarations[token.value]) != doubleAttrTokens.end() && find(intermediate[0].begin(), intermediate[0].end(), token.value) != intermediate[0].end()) {
        int index = -1;

        // Get the column index of the synonym
        for (int i = 0; i < intermediate[0].size(); i++) {
            if (intermediate[0][i] == token.value) {
                index = i;
            }
        }

        intermediate[0].push_back(updatedColumnName(token));

        if (token.type == TokenType::CALL) {
            map<int, string> ProcedureStmtNumToName = servicer->retrieveCallStmtProcMapping();

            for (int i = 1; i < intermediate.size(); i++) {
                // insert to get procedure name for each call statement
                intermediate[i].push_back(ProcedureStmtNumToName[stoi(intermediate[i][index])]);
            }
        } else {
            StmtVarRelationType sv = declarations[token.value] == TokenType::READ ? StmtVarRelationType::MODIFIESSV : StmtVarRelationType::USESSV;

            for (int i = 1; i < intermediate.size(); i++) {
                // We should only get one variable for Call, Print, Read
                intermediate[i].push_back(servicer->forwardRetrieveRelation(stoi(intermediate[i][index]), sv)[0]);
            }
        }

        return true;
    }

    return false;
}

vector<string> FilterEvaluator::selectAll(const TokenType type)
{
    vector<string> result;

    if (type == TokenType::VARIABLE) {
        for (NameExpression v : servicer->getAllVar()) {
            result.push_back(v.getVarName());
        }
    } else if (type == TokenType::CONSTANT) {
        for (ConstantExpression c : servicer->getAllConst()) {
            result.push_back(to_string(c.getValue()));
        }
    } else if (type == TokenType::PROCEDURE) {
        for (Procedure p : servicer->getAllProc()) {
            result.push_back(p.getProcedureName());
        }
    } else {
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