using namespace std;

#include <string>
#include <set>
#include <memory>

#include "QueryEvaluator.h"
#include "QPS.h"

#include "../AST/TNode.h"
#include "../PKB/PKB.h"
#include "../PKB/QueryServicer.h"


unordered_map<TokenType, StatementType> tokenTypeToStatementType = {
    { TokenType::STATEMENT, StatementType::STATEMENT},
    { TokenType::ASSIGN, StatementType::ASSIGN },
    { TokenType::CALL, StatementType::CALL },
    { TokenType::IF, StatementType::IF },
    { TokenType::PRINT, StatementType::PRINT },
    { TokenType::READ, StatementType::READ },
    { TokenType::WHILE, StatementType::WHILE }
};

unordered_map<TokenType, StmtVarRelationType> tokenTypeToStmtVarRelationType = {
    { TokenType::USES, StmtVarRelationType::USESV},
    { TokenType::USES_P, StmtVarRelationType::USESV },
    { TokenType::MODIFIES, StmtVarRelationType::MODIFIESV },
    { TokenType::MODIFIES_P, StmtVarRelationType::MODIFIESV },
};

unordered_map<TokenType, StmtStmtRelationType> tokenTypeToStmtStmtRelationType = {
    { TokenType::FOLLOWS, StmtStmtRelationType::FOLLOWS},
    { TokenType::FOLLOWS_A, StmtStmtRelationType::FOLLOWSS },
    { TokenType::PARENT, StmtStmtRelationType::PARENT },
    { TokenType::PARENT_A, StmtStmtRelationType::PARENTS },
};


QueryEvaluator::QueryEvaluator(PqlQuery& pqlQuery, shared_ptr<QueryServicer> s, list<string>& r) :
    result(r), servicer(s), pq(pqlQuery) {}

void QueryEvaluator::evaluate() {
    const string selectSynonym = pq.select;
    const TokenType type = pq.declarations[selectSynonym];

    // If there are no clauses / select synonym not in any clauses
    if (checkIfClauseExists() || checkIfSelectSynonymExistsInClause) {
        selectAll(type);
        return;
    }

    // Using multimap in order to join our tables
    vector<vector<string>> patternTable;
    vector<vector<string>> suchThatTable;

    evaluatePatternClause(patternTable);
    bool pass = evaluateSuchThatClause(suchThatTable);

    if (!pass) {
        return;
    }

    // Combine the two tables
    // We can hardcode this part since maximum of 4 variables due to max 1 such-that, 1 pattern
    
    // find two common variables from 2 tables 
    //
}

/*
Returns me a vector of:
[[assignment synonym, any variable synonym in the first arg],
 [ stmt1 , var1],
 [ stmt1 , var2],
 and so on
*/
void QueryEvaluator::evaluatePatternClause(vector<vector<string>> & intermediate) {
    // Initalize our first row of headers
    intermediate.push_back(vector<string>());

    for (Clause clause : pq.patternClauses) {
        string leftValue = clause.left;
        string rightValue = clause.right;
        intermediate[0].push_back(clause.clauseType.value);

        if (leftValue == "_" && rightValue == "_") {
            // Update code in the future for the different
            set<shared_ptr<Statement>> statements = servicer->getAllStmt(StatementType::ASSIGN);
            intermediate.push_back(vector<string> {clause.clauseType.value});

            for (shared_ptr<Statement> s : statements) {
                intermediate.push_back(vector<string> {to_string(s->getLineNum())});
            }
        }

        else if (leftValue == "_" && rightValue != "_") {
            vector<int> result = servicer->reverseRetrieveRelation(rightValue, StmtVarRelationType::USESV);

            for (int stmtNum : result) {
                intermediate.push_back(vector<string> {to_string(stmtNum)});
            }
        }

        else if (leftValue != "_" && rightValue == "_") {
            vector<int> result = servicer->reverseRetrieveRelation(leftValue, StmtVarRelationType::MODIFIESV);
            
            if (pq.declarations[leftValue] == TokenType::VARIABLE) {
                intermediate[0].push_back(leftValue);
            }

            for (int i : result) {
                if (pq.declarations[leftValue] == TokenType::VARIABLE) {
                    vector<string> variables = servicer->forwardRetrieveRelation(i, StmtVarRelationType::MODIFIESV);

                    for (string v : variables) {
                        intermediate.push_back(vector<string> {to_string(i), v});
                    }
                }
                else {
                    intermediate.push_back(vector<string> {to_string(i)});
                }
            }
        }

        else {
            vector<int> stmtWithRightArg = servicer->reverseRetrieveRelation(rightValue, StmtVarRelationType::USESV);

            if (pq.declarations[leftValue] != TokenType::VARIABLE) {
                vector<int> stmtWithLeftArg = servicer->reverseRetrieveRelation(leftValue, StmtVarRelationType::MODIFIESV);
                vector<int> assignStmts;

                // Gets the intersections of the two list of assign stmt numbers
                sort(stmtWithLeftArg.begin(), stmtWithLeftArg.end());
                sort(stmtWithRightArg.begin(), stmtWithRightArg.end());

                set_intersection(stmtWithLeftArg.begin(), stmtWithLeftArg.end(),
                    stmtWithRightArg.begin(), stmtWithRightArg.end(),
                    back_inserter(assignStmts));


                for (int i : assignStmts) {
                    intermediate.push_back(vector<string> {to_string(i)});
                }
            }

            else {
                intermediate[0].push_back(leftValue);

                for (int i : stmtWithRightArg) {
                    vector<string> variables = servicer->forwardRetrieveRelation(i, StmtVarRelationType::MODIFIESV);

                    for (string v : variables) {
                        intermediate.push_back(vector<string> {to_string(i), v});
                    }
                }
            }
        }
    }
}


// Filters a list of statement numbers
// Boolean indicates if filtering is successful (mostly in the case of 2 value arguments)
// If false, we can immediately return an empty vector for result

// Otherwise, like pattern, we return a vector

bool QueryEvaluator::evaluateSuchThatClause(vector<vector<string>>& intermediate) {
    for (Clause clause : pq.suchThatClauses) {
        TokenType leftType = pq.declarations[clause.left];
        TokenType rightType = pq.declarations[clause.right];
        string leftValue = clause.left;
        string rightValue = clause.right;
        TokenType clauseType = clause.clauseType.type;

        // If Both arguments are NOT synonyms
        if (leftType != TokenType::SYNONYM && rightType != TokenType::SYNONYM) {

            // If right arg is a variable aka Uses/Modifies
            if (rightType == TokenType::STRING) {
                StmtVarRelationType sv = tokenTypeToStmtVarRelationType[clauseType];

                if (leftType == TokenType::STATEMENT_NUM) {
                    return servicer->retrieveRelation(stoi(leftValue), rightValue, sv);
                }

                // This is for procedures but not covered in milestone 1
                else {}
            }

            // if right side is statement number aka Follows/Parents
            else if (rightType == TokenType::STATEMENT_NUM) {
                StmtStmtRelationType ss = tokenTypeToStmtStmtRelationType[clauseType];

                if (leftType == TokenType::STATEMENT_NUM) {
                    return servicer->retrieveRelation(stoi(leftValue), stoi(rightValue), ss);
                }

                // If left side is a wildcard, returns a list of statement numbers as well
                else {
                    intermediate.push_back(vector<string> {leftValue});
                    vector<int> stmtNums = servicer->reverseRetrieveRelation(stoi(rightValue), ss);

                    for (int i : stmtNums) {
                        intermediate.push_back(vector<string> { to_string(i) });
                    }

                    return true;
                }
            }

            // If right side is a wildcard
            else {
                if (leftType == TokenType::STATEMENT_NUM &&
                    (clauseType == TokenType::FOLLOWS || clauseType == TokenType::FOLLOWS_A ||
                        clauseType == TokenType::PARENT || clauseType == TokenType::PARENT_A)) {
                    StmtStmtRelationType ss = tokenTypeToStmtStmtRelationType[clauseType];
                    return !servicer->forwardRetrieveRelation(stoi(leftValue), ss).empty();
                }
                else {
                    StmtVarRelationType sv = tokenTypeToStmtVarRelationType[clauseType];
                    return !servicer->forwardRetrieveRelation(stoi(leftValue), sv).empty();
                }
            }
        }

        // If left side is a synonym and right is NOT synonym
        else if (leftType == TokenType::SYNONYM && rightType != TokenType::SYNONYM) {
            intermediate.push_back(vector<string> {leftValue});

            if (clauseType == TokenType::FOLLOWS || clauseType == TokenType::FOLLOWS_A ||
                clauseType == TokenType::PARENT || clauseType == TokenType::PARENT_A) {

                StmtStmtRelationType ss = tokenTypeToStmtStmtRelationType[clauseType];

                if (rightType == TokenType::STATEMENT_NUM) {
                    vector<int> result = servicer->reverseRetrieveRelation(stoi(rightValue), ss);

                    for (int i : result) {
                        intermediate.push_back(vector<string> { to_string(i) });
                    }
                }
                // if right side is a wildcard
                else {
                    set<shared_ptr<Statement>> result = servicer->getAllStmt(tokenTypeToStatementType[leftType]);

                    for (shared_ptr<Statement> s : result) {
                        if (!servicer->forwardRetrieveRelation(s->getLineNum(), ss).empty()) {
                            intermediate.push_back(vector<string>{ to_string(s->getLineNum()) });
                        }
                    }
                }
            }
            // Uses/Modifies
            else {
                StmtVarRelationType sv = tokenTypeToStmtVarRelationType[clauseType];

                if (rightType == TokenType::STRING) {
                    vector<int> result = servicer->reverseRetrieveRelation(rightValue, sv);

                    for (int i : result) {
                        intermediate.push_back(vector<string> { to_string(i) });
                    }
                }
                // if right side is a wildcard
                else {
                    set<shared_ptr<Statement>> result = servicer->getAllStmt(tokenTypeToStatementType[leftType]);

                    for (shared_ptr<Statement> s : result) {
                        if (!servicer->forwardRetrieveRelation(s->getLineNum(), sv).empty()) {
                            intermediate.push_back(vector<string>{ to_string(s->getLineNum()) });
                        }
                    }
                }
            }
        }

        // If left side is NOT a synonym and right is synonym
        else if (leftType != TokenType::SYNONYM && rightType == TokenType::SYNONYM) {
            intermediate.push_back(vector<string> {rightValue});

            if (clauseType == TokenType::FOLLOWS || clauseType == TokenType::FOLLOWS_A ||
                clauseType == TokenType::PARENT || clauseType == TokenType::PARENT_A) {

                StmtStmtRelationType ss = tokenTypeToStmtStmtRelationType[clauseType];

                if (leftType == TokenType::STATEMENT_NUM) {
                    vector<int> result = servicer->forwardRetrieveRelation(stoi(leftValue), ss);

                    for (int i : result) {
                        intermediate.push_back(vector<string> { to_string(i) });
                    }
                }
                // if left side is a wildcard
                else {
                    set<shared_ptr<Statement>> result = servicer->getAllStmt(tokenTypeToStatementType[rightType]);

                    for (shared_ptr<Statement> s : result) {
                        if (!servicer->reverseRetrieveRelation(s->getLineNum(), ss).empty()) {
                            intermediate.push_back(vector<string>{ to_string(s->getLineNum()) });
                        }
                    }
                }
            }
            else {
                StmtVarRelationType sv = tokenTypeToStmtVarRelationType[clauseType];

                if (leftType == TokenType::STATEMENT_NUM) {
                    vector<string> result = servicer->forwardRetrieveRelation(stoi(leftValue), sv);

                    for (string s : result) {
                        intermediate.push_back(vector<string> { s });
                    }
                }
            }
        }

            // Both are synonyms
        else {
            intermediate.push_back(vector<string>{ leftValue, rightValue });
            set<shared_ptr<Statement>> left = servicer->getAllStmt(tokenTypeToStatementType[leftType]);

            if (clauseType == TokenType::FOLLOWS || clauseType == TokenType::FOLLOWS_A ||
                clauseType == TokenType::PARENT || clauseType == TokenType::PARENT_A) {

                StmtStmtRelationType ss = tokenTypeToStmtStmtRelationType[clauseType];

                for (shared_ptr<Statement> s : left) {
                    for (int i : servicer->forwardRetrieveRelation(s->getLineNum(), ss)) {
                        intermediate.push_back(vector<string>
                        { to_string(s->getLineNum()), to_string(i) });
                    }
                }
            }
            else {
                StmtVarRelationType sv = tokenTypeToStmtVarRelationType[clauseType];

                for (shared_ptr<Statement> s : left) {
                    for (string v : servicer->forwardRetrieveRelation(s->getLineNum(), sv)) {
                        intermediate.push_back(vector<string>
                        { to_string(s->getLineNum()), v });
                    }
                }
            }
        }
    }
}




void QueryEvaluator::selectAll(TokenType type) {
    if (type == TokenType::VARIABLE) {
        set<NameExpression> variables = servicer->getAllVar();

        for (NameExpression v : variables) {
            result.push_back(v.getVarName());
        }
    }

    else if (type == TokenType::CONSTANT) {
        set<ConstantExpression> constants = servicer->getAllConst();

        for (ConstantExpression c : constants) {
            result.push_back(to_string(c.getValue()));
        }
    }

    else {
        if (tokenTypeTostatementType.find(type) != tokenTypeTostatementType.end()) {
            StatementType stmtType = tokenTypeTostatementType[type];
            set<shared_ptr<Statement>> statements = servicer->getAllStmt(stmtType);
            
            for (shared_ptr<Statement> s : statements) {
                result.push_back(to_string(s->getLineNum()));
            }
        }
    }
}

inline bool QueryEvaluator::checkIfClauseExists() {
    return pq.patternClauses.empty() && pq.suchThatClauses.empty();
}

bool QueryEvaluator::checkIfSelectSynonymExistsInClause() {
    for (auto clause : pq.suchThatClauses) {
        if (pq.select == clause.left || pq.select == clause.right) {
            return true;
        }
    }

    for (auto clause : pq.patternClauses) {
        if (pq.select == clause.clauseType.value || 
            pq.select == clause.left || pq.select == clause.right) {
            return true;
        }
    }

    return false;
}