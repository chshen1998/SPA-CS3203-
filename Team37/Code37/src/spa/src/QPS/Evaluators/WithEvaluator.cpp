using namespace std;

#include "QPS/Structures/PqlQuery.h"
#include "QPS/Structures/PqlToken.h"
#include "QPS/Types/TokenType.h"
#include "WithEvaluator.h"
#include "PKB/Types/StatementType.h"
#include "PKB/Types/StmtVarRelationType.h"
#include "EvaluatorUtils.h"

using namespace EvaluatorUtils;

set<TokenType> doubleAttrTokens = {
    TokenType::CALL, TokenType::PRINT, TokenType::READ
};

vector<vector<string>> WithEvaluator::evaluateClause(const Clause& clause, vector<vector<string>> intermediate)
{
    PqlToken leftArg = clause.left;
    PqlToken rightArg = clause.right;
    vector<vector<string>> finalResult;

    // If initial table is empty
    // Eg. Query only consists of one WITH clause
    if (intermediate.empty()) {
        fillInitialTable(clause, intermediate);
    }

    // Check if current table does not consists of with arguments
    if (find(intermediate[0].begin(), intermediate[0].end(), leftArg.value) == intermediate[0].end()) {
        vector<vector<string>> temp;
        fillInitialTable(clause, temp);

        intermediate = JoinTable(intermediate, temp);
    }

    // Add procName for call, print, and read
    bool isLeftDoubleAttr = WithEvaluator::addProcName(intermediate, leftArg);
    string leftValue = isLeftDoubleAttr ? updatedColumnName(leftArg) : leftArg.value;
    finalResult.push_back(intermediate[0]);

    // Two Synonyms - s.procName() = v.procName()
    if (clause.rightAttr.type != TokenType::NONE) {        
        bool isRightDoubleAttr = WithEvaluator::addProcName(intermediate, rightArg);

        // Insert Column headers
        int leftArgIndex = -1;
        int rightArgIndex = -1;

        string rightValue = isRightDoubleAttr ? updatedColumnName(rightArg) : rightArg.value;

        for (int i = 0; i < intermediate[0].size(); i++) {
            if (intermediate[0][i] == leftValue) {
                leftArgIndex = i;
            }

            if (intermediate[0][i] == rightValue) {
                rightArgIndex = i;
            }
        }

        for (int j = 1; j < intermediate.size(); j++) {
            if (intermediate[j][leftArgIndex] == intermediate[j][rightArgIndex]) {
                finalResult.push_back(intermediate[j]);
            }
        }
    }

    // intermediate: c, v (stmtnums, var)
    // One Synonyms - c.procName() = "main"
    else {
        int leftArgIndex = -1;

        for (int i = 0; i < intermediate[0].size(); i++) {
            if (intermediate[0][i] == leftValue) {
                leftArgIndex = i;
            }
        }

        for (int j = 1; j < intermediate.size(); j++) {
            if (intermediate[j][leftArgIndex] == rightArg.value) {
                finalResult.push_back(intermediate[j]);
            }
        }
    }

    return finalResult;
}

/*
* For cases with 1 = 1 or "x" = "x"
*/
bool WithEvaluator::evaluateBooleanClause(const Clause& clause) {
    return clause.left == clause.right;
}

bool WithEvaluator::addProcName(vector<vector<string>>& intermediate, const PqlToken& token) {
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

inline string WithEvaluator::updatedColumnName(const PqlToken& token) {
    return token.type == TokenType::CALL ? token.value + ".ProcName" : token.value + ".VarName";
}


void WithEvaluator::fillInitialTable(const Clause& clause, vector<vector<string>>& intermediate) {
    intermediate.push_back(vector<string> { clause.left.value });

    if (clause.right.type == TokenType::SYNONYM) {
        intermediate[0].push_back(clause.right.value);

        for (string left : selectAll(declarations[clause.left.value])) {
            for (string right : selectAll(declarations[clause.right.value])) {
                intermediate.push_back(vector<string> { left, right });
            }
        }
    }
    else {
        for (string left : selectAll(declarations[clause.left.value])) {
            intermediate.push_back(vector<string> { left });
        }
    }
}


vector<string> WithEvaluator::selectAll(TokenType type) {
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

