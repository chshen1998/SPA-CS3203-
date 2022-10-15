using namespace std;

#include "QPS/Structures/PqlQuery.h"
#include "QPS/Structures/PqlToken.h"
#include "QPS/Types/TokenType.h"
#include "FilterEvaluator.h"
#include "PKB/Types/StatementType.h"
#include "PKB/Types/StmtVarRelationType.h"
#include "QPS/Evaluators/EvaluatorUtils.h"

using namespace EvaluatorUtils;


inline string FilterEvaluator::updatedColumnName(const PqlToken& token) {
    return token.type == TokenType::CALL ? token.value + ".ProcName" : token.value + ".VarName";
}


bool FilterEvaluator::addProcName(vector<vector<string>>& intermediate, const PqlToken& token) {
    string updated = updatedColumnName(token);
    if (doubleAttrTokens.find(declarations[token.value]) != doubleAttrTokens.end() &&
        find(intermediate[0].begin(), intermediate[0].end(), token.value) != intermediate[0].end()) {
        int index = -1;

        // Get the column index of the synonym
        for (int i = 0; i < intermediate[0].size(); i++) {
            if (intermediate[0][i] == token.value) {
                index = i;
            }
        }

        intermediate[0].push_back(updated);

        if (token.type == TokenType::CALL) {
            for (int i = 1; i < intermediate.size(); i++) {
                // insert to get procedure name for each call statement
            }
        }
        else {
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


vector<string> FilterEvaluator::selectAll(const TokenType type) {
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