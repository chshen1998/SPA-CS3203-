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

#include "AST/Expression/RelationalFactor/NameExpression.h"
#include "AST/Expression/RelationalFactor/ConstantExpression.h"
#include "AST/Statement/Statement.h"
#include "PKB/PKB.h"
#include "PKB/QueryServicer.h"
#include "PKB/Types/StatementType.h"
#include "PKB/Types/StmtStmtRelationType.h"
#include "PKB/Types/StmtVarRelationType.h"


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
    { TokenType::USES, StmtVarRelationType::USESSV},
    { TokenType::USES_P, StmtVarRelationType::USESSV },
    { TokenType::MODIFIES, StmtVarRelationType::MODIFIESSV },
    { TokenType::MODIFIES_P, StmtVarRelationType::MODIFIESSV },
};

unordered_map<TokenType, StmtStmtRelationType> tokenTypeToStmtStmtRelationType = {
    { TokenType::FOLLOWS, StmtStmtRelationType::FOLLOWS},
    { TokenType::FOLLOWS_A, StmtStmtRelationType::FOLLOWSS },
    { TokenType::PARENT, StmtStmtRelationType::PARENT },
    { TokenType::PARENT_A, StmtStmtRelationType::PARENTS },
};

set<TokenType> suchThatStmtRefStmtRef = {
    TokenType::FOLLOWS, TokenType::FOLLOWS_A, TokenType::PARENT, TokenType::PARENT_A
};


QueryEvaluator::QueryEvaluator(PqlQuery& pqlQuery, shared_ptr<QueryServicer> s, list<string>& r) :
    result(r), servicer(s), pq(pqlQuery) {}

void QueryEvaluator::evaluate() {
    const string selectSynonym = pq.select;
    const TokenType type = pq.declarations[selectSynonym];
 
    // If there are no clauses / select synonym not in any clauses
    if (checkIfClauseNoExists() || !checkIfSelectSynonymExistsInClause()) {
        selectAll(type);
        return;
    }

    // Using multimap in order to join our tables
    vector<vector<string>> patternTable;
    vector<vector<string>> suchThatTable;

    evaluatePatternClause(patternTable);

    // 1 Denotes true, -1 denote false, 0 if we are getting a table
    int boolSuchThat = evaluateSuchThatClause(suchThatTable);

    if (boolSuchThat == -1) {
        return;
    }

    // If Pattern Clause only
    if (pq.patternClauses.size() == 1 && pq.suchThatClauses.size() == 0) {
        getResultFromFinalTable(patternTable);

        if (result.empty()) {
            return;
        }
    }

    // If Such that Clause Only
    else if (pq.patternClauses.size() == 0 && pq.suchThatClauses.size() == 1) {
        if (boolSuchThat == 0) {
            getResultFromFinalTable(suchThatTable);

            if (result.empty()) {
                return;
            }
        }
        else {
            selectAll(type);
        }
    }
    // Combine the two tables
    // We can hardcode this part since maximum of 4 variables due to max 1 such-that, 1 pattern
    else {
        int patternIndex = -1;
        int suchThatIndex = -1;
        bool useSelectSynonym = false;
        bool inPattern = false;

        vector<string> patternHeader = patternTable[0];
        vector<string> suchThatHeader = suchThatTable[0];
        sort(patternHeader.begin(), patternHeader.end());
        sort(suchThatHeader.begin(), suchThatHeader.end());

        // case where Uses(a, v) pattern a(v, "_")
        // We find the select Synonym and use that as our common column
        if (patternHeader == suchThatHeader) {
            useSelectSynonym = true;
        }

        bool breakChecker = false;

        // find a common synonym
        for (int i = 0; i < patternTable[0].size(); i++) {
            for (int j = 0; j < suchThatTable[0].size(); j++) {
                if (patternTable[0][i] == suchThatTable[0][j]) {
                    patternIndex = i;
                    suchThatIndex = j;

                    if (patternTable[0][i] == selectSynonym && useSelectSynonym) {
                        breakChecker = true;
                        break;
                    }
                }
            }

            if (breakChecker) {
                break;
            }

            // If pattern clause and such that clause do not have a common pattern,
            // Example: Select v such that Uses(2, v) pattern a ("x", _ ) <-- we only check user
            if (!inPattern && patternTable[0][i] == selectSynonym) {
                inPattern = true;
            }
        }

        if (patternIndex != -1 && suchThatIndex != -1) {
            vector<vector<string>> finalTable = joinTwoTable(patternTable, patternIndex, suchThatTable, suchThatIndex);
            getResultFromFinalTable(finalTable);
        }

        else if (inPattern) {
            getResultFromFinalTable(patternTable);
        }

        else {
            getResultFromFinalTable(suchThatTable);
        }
    }
}


vector<vector<string>> QueryEvaluator::joinTwoTable(const vector<vector<string>>& v1, size_t columnIndex1,
    const vector<vector<string>>& v2, size_t columnIndex2)
{
    vector<vector<string>> result;
    vector<string> columns;

    string commonColumn = v1[0][columnIndex1];

    for (string s : v1[0]) {
        columns.push_back(s);
    }
    for (string s : v2[0]) {
        if (s == commonColumn) continue;
        columns.push_back(s);
    }

    // push headers
    result.push_back(columns);

    for (int i = 1; i != v1.size(); i++) {

        string joiner = v1[i][columnIndex1];

        for (int j = 1; j != v2.size(); j++) {
            if (v2[j][columnIndex2] == joiner) {

                // push a new joined row
                vector<string> temp;

                for (string s1 : v1[i]) {
                    temp.push_back(s1);
                }
                for (auto k = 0; k != v2[j].size(); k++) {
                    if (k == columnIndex2) continue;
                    temp.push_back(v2[j][k]);
                }

                result.push_back(temp);
            }
        }
    }

    return result;
}


void QueryEvaluator::getResultFromFinalTable(const vector<vector<string>>& table) {
    int index = 0;
    // Find the column index where the synonym value == select synonym value
    for (int i = 0; i < table[0].size(); i++) {
        if (table[0][i] == pq.select) {
            index = i;
            break;
        }
    }

    // Add the result values of that column into result
    for (int j = 1; j < table.size(); j++) {
        if (find(result.begin(), result.end(), table[j][index]) == result.end()) {
            result.push_back(table[j][index]);
        }
    }

    return;
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
        intermediate[0].push_back(clause.clauseType.value);

        PqlToken leftArg = clause.left;
        PqlToken rightArg = clause.right;
        set<shared_ptr<Statement>> allAssignStmts = servicer->getAllStmt(StatementType::ASSIGN);
        vector<int> allAssignStmtLines;

        for (shared_ptr<Statement> s : allAssignStmts) {
            allAssignStmtLines.push_back(s->getLineNum());
        }

        if (leftArg.type == TokenType::WILDCARD && rightArg.type == TokenType::WILDCARD) {
            for (shared_ptr<Statement> s : allAssignStmts) {
                intermediate.push_back(vector<string> { to_string(s->getLineNum()) });
            }
        }

        else if (leftArg.type == TokenType::WILDCARD && rightArg.type != TokenType::WILDCARD) {
            string parsed = rightArg.type == TokenType::WILDCARD_STRING ?
                            rightArg.value.substr(2, rightArg.value.size() - 4) :
                            rightArg.value.substr(1, rightArg.value.size() - 2);
            
            vector<int> finalResult;
            vector<int> allStmtWithRightArg = servicer->reverseRetrieveRelation(parsed, StmtVarRelationType::USESSV);
            
            getListOfStmtNumbersIntersection(finalResult, allStmtWithRightArg, allAssignStmtLines);
           
            for (int lines : finalResult) {
                intermediate.push_back(vector<string> { to_string(lines) });
            }
        }

        else if (leftArg.type != TokenType::WILDCARD && rightArg.type == TokenType::WILDCARD) {
            
            // If leftArg.value is a string
            if (leftArg.type == TokenType::STRING) {
                string value = leftArg.value.substr(1, leftArg.value.size() - 2);

                vector<int> finalResult;
                vector<int> allStmtWithLeftArg = servicer->reverseRetrieveRelation(value, StmtVarRelationType::MODIFIESSV);

                getListOfStmtNumbersIntersection(finalResult, allStmtWithLeftArg, allAssignStmtLines);

                for (int lines : finalResult) {
                    intermediate.push_back(vector<string> { to_string(lines) });
                }
            }

            // if leftArg is a synonym
            // pattern a ( v, _); --> Get list of assign and their variable synonyms
            else {
                intermediate[0].push_back(leftArg.value);

                for (shared_ptr<Statement> s : allAssignStmts) {
                    string v = servicer->forwardRetrieveRelation(s->getLineNum(), StmtVarRelationType::MODIFIESSV)[0];
                    intermediate.push_back(vector<string> {to_string(s->getLineNum()), v });
                }
            }   
        }

        // If both not wildcards
        else {
            string rightArgParsed = rightArg.type == TokenType::WILDCARD_STRING ?
                                    rightArg.value.substr(2, rightArg.value.size() - 4) :
                                    rightArg.value.substr(1, rightArg.value.size() - 2);

            vector<int> stmtWithRightArg = servicer->reverseRetrieveRelation(rightArgParsed, StmtVarRelationType::USESSV);

            // pattern a ("x", _"y"_) -> Get intersection with "x" on LHS, and _"y"_ on RHS and all assignment statements
            if (leftArg.type != TokenType::SYNONYM) {
                string leftArgParsed = leftArg.value.substr(1, leftArg.value.size() - 2);

                vector<int> stmtWithLeftArg = servicer->reverseRetrieveRelation(leftArgParsed, StmtVarRelationType::MODIFIESSV);
                vector<int> assignStmts;
                vector<int> finalResult;

                getListOfStmtNumbersIntersection(assignStmts, stmtWithLeftArg, stmtWithRightArg);
                getListOfStmtNumbersIntersection(finalResult, assignStmts, allAssignStmtLines);

                for (int i : finalResult) {
                    intermediate.push_back(vector<string> {to_string(i)});
                }
            }

            // pattern a (v, _"y"_) -> Get intersection of _"y"_ on RHS, and all assignment statements
            else {
                intermediate[0].push_back(leftArg.value);
                vector<int> finalResult;
                getListOfStmtNumbersIntersection(finalResult, stmtWithRightArg, allAssignStmtLines);

                for (int i : finalResult) {
                    vector<string> variables = servicer->forwardRetrieveRelation(i, StmtVarRelationType::MODIFIESSV);

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

int QueryEvaluator::evaluateSuchThatClause(vector<vector<string>>& intermediate) {
    for (Clause clause : pq.suchThatClauses) {
        PqlToken leftArg = clause.left;
        PqlToken rightArg = clause.right;
        TokenType clauseType = clause.clauseType.type;

        // If Both arguments are NOT synonyms
        if (leftArg.type != TokenType::SYNONYM && rightArg.type != TokenType::SYNONYM) {

            // If right arg is a variable aka Uses/Modifies
            if (rightArg.type == TokenType::STRING) {
                StmtVarRelationType sv = tokenTypeToStmtVarRelationType[clauseType];

                // Remove the inverted commas
                string parsed = rightArg.value.substr(1, rightArg.value.size() - 2);

                // Example: Uses(1, "x") / Modifies(1, "x")
                if (leftArg.type == TokenType::STATEMENT_NUM) {
                    return servicer->retrieveRelation(stoi(leftArg.value), parsed, sv) ? 1 : -1;
                }

                // This is for procedures but not covered in milestone 1
                //else {}
            }

            // if right side is statement number aka Follows/Parents
            else if (rightArg.type == TokenType::STATEMENT_NUM) {
                StmtStmtRelationType ss = tokenTypeToStmtStmtRelationType[clauseType];

                // Example: Follows(1,2) / Parents(1,2)
                if (leftArg.type == TokenType::STATEMENT_NUM) {
                    return servicer->retrieveRelation(stoi(leftArg.value), stoi(rightArg.value), ss) ? 1 : -1;
                }

                // If left side is a wildcard, returns a list of statement numbers as well
                // Follows(_, 5) -> return boolean since there is no synonyms
                // Parents(_, 10) 
                else {
                    intermediate.push_back(vector<string> {leftArg.value});
                    vector<int> stmtNums = servicer->reverseRetrieveRelation(stoi(rightArg.value), ss);

                    return !stmtNums.empty() ? 1 : -1;
                }
            }

            // If right side is a wildcard,
            // We cannot have Such that clause with two wildcards (too ambiguous)
            else {

                // Example: Follows(6, _)
                if (leftArg.type == TokenType::STATEMENT_NUM &&
                    suchThatStmtRefStmtRef.find(clauseType) != suchThatStmtRefStmtRef.end()) {

                    StmtStmtRelationType ss = tokenTypeToStmtStmtRelationType[clauseType];
                    return !servicer->forwardRetrieveRelation(stoi(leftArg.value), ss).empty() ? 1 : -1;
                }

                // Example: Uses(1, _)
                else {
                    StmtVarRelationType sv = tokenTypeToStmtVarRelationType[clauseType];
                    return !servicer->forwardRetrieveRelation(stoi(leftArg.value), sv).empty() ? 1 : -1;
                }
            }
        }

        // If left side is a synonym and right is NOT synonym
        else if (leftArg.type == TokenType::SYNONYM && rightArg.type != TokenType::SYNONYM) {
            intermediate.push_back(vector<string> {leftArg.value});

            StatementType st = tokenTypeToStatementType[pq.declarations[leftArg.value]];
            set<shared_ptr<Statement>> allStmtOfLeftArgDeclaration = servicer->getAllStmt(st);
            vector<int> allStmtLinesOfLeftArgDeclaration;

            for (shared_ptr<Statement> s : allStmtOfLeftArgDeclaration) {
                allStmtLinesOfLeftArgDeclaration.push_back(s->getLineNum());
            }

            if (suchThatStmtRefStmtRef.find(clauseType) != suchThatStmtRefStmtRef.end()) {
                // We get all Statement Numbers of the specific Left Arg Argument (if leftArg is an if statement, we get all if statements)
                StmtStmtRelationType ss = tokenTypeToStmtStmtRelationType[clauseType];

                // Example: Follows(s, 6)
                // get intersection of all statements that follow 6, and specific type of s
                if (rightArg.type == TokenType::STATEMENT_NUM) {
                    vector<int> allStmtWithRightArg = servicer->reverseRetrieveRelation(stoi(rightArg.value), ss);
                    vector<int> finalResult;

                    getListOfStmtNumbersIntersection(finalResult, allStmtLinesOfLeftArgDeclaration, allStmtWithRightArg);

                    for (int i : finalResult) {
                        intermediate.push_back(vector<string> { to_string(i) });
                    }
                }
                // If right side is a wildcard
                // Example: Follows(s, _)
                else {
                    for (int lines : allStmtLinesOfLeftArgDeclaration) {
                        if (!servicer->forwardRetrieveRelation(lines, ss).empty()) {
                            intermediate.push_back(vector<string>{ to_string(lines) });
                        }
                    }
                }
            }

            // Uses/Modifies
            else {
                StmtVarRelationType sv = tokenTypeToStmtVarRelationType[clauseType];

                // Example: Uses(s, "x") <- s is a statement
                if (rightArg.type == TokenType::STRING) {
                    // Remove the inverted commas
                    string parsed = rightArg.value.substr(1, rightArg.value.size() - 2);
                    vector<int> allStmtWithRightArg = servicer->reverseRetrieveRelation(parsed, sv);
                    vector<int> finalResult;

                    getListOfStmtNumbersIntersection(finalResult, allStmtLinesOfLeftArgDeclaration, allStmtWithRightArg);

                    for (int i : finalResult) {
                        intermediate.push_back(vector<string> { to_string(i) });
                    }
                }
                // if right side is a wildcard
                else {
                    for (int lines : allStmtLinesOfLeftArgDeclaration) {
                        if (!servicer->forwardRetrieveRelation(lines, sv).empty()) {
                            intermediate.push_back(vector<string>{ to_string(lines) });
                        }
                    }
                }
            }
        }

        // If left side is NOT a synonym and right is synonym
        else if (leftArg.type != TokenType::SYNONYM && rightArg.type == TokenType::SYNONYM) {
            intermediate.push_back(vector<string> {rightArg.value});

            StatementType st = tokenTypeToStatementType[pq.declarations[rightArg.value]];
            set<shared_ptr<Statement>> allStmtOfRightArgDeclaration = servicer->getAllStmt(st);
            vector<int> allStmtLinesOfRightArgDeclaration;

            for (shared_ptr<Statement> s : allStmtOfRightArgDeclaration) {
                allStmtLinesOfRightArgDeclaration.push_back(s->getLineNum());
            }

            if (suchThatStmtRefStmtRef.find(clauseType) != suchThatStmtRefStmtRef.end()) {
                StmtStmtRelationType ss = tokenTypeToStmtStmtRelationType[clauseType];

                // Example: Follows(1, s)
                if (leftArg.type == TokenType::STATEMENT_NUM) {
                    vector<int> allStmtWithLeftArg = servicer->forwardRetrieveRelation(stoi(leftArg.value), ss);
                    vector<int> finalResult;

                    getListOfStmtNumbersIntersection(finalResult, allStmtLinesOfRightArgDeclaration, allStmtWithLeftArg);

                    for (int i : finalResult) {
                        intermediate.push_back(vector<string> { to_string(i) });
                    }
                }
                // if left side is a wildcard
                // Example: Follows(_, s)
                else {
                    for (int lines : allStmtLinesOfRightArgDeclaration) {
                        if (!servicer->reverseRetrieveRelation(lines, ss).empty()) {
                            intermediate.push_back(vector<string>{ to_string(lines) });
                        }
                    }
                }
            }
            else {
                // Example: Uses(2, v)
                StmtVarRelationType sv = tokenTypeToStmtVarRelationType[clauseType];

                if (leftArg.type == TokenType::STATEMENT_NUM) {
                    vector<string> allVariablesWithLeftArg = servicer->forwardRetrieveRelation(stoi(leftArg.value), sv);

                    for (string s : allVariablesWithLeftArg) {
                        intermediate.push_back(vector<string> { s });
                    }
                }

                // Procedure
                else {}
            }
        }

        // Both are synonyms
        else {
            intermediate.push_back(vector<string>{ leftArg.value, rightArg.value });

            // Get all the stmt number of type first arg
            StatementType st = tokenTypeToStatementType[pq.declarations[leftArg.value]];
            set<shared_ptr<Statement>> allStmtOfLeftArgDeclaration = servicer->getAllStmt(st);
            vector<int> allStmtLinesOfLeftArgDeclaration;

            for (shared_ptr<Statement> s : allStmtOfLeftArgDeclaration) {
                allStmtLinesOfLeftArgDeclaration.push_back(s->getLineNum());
            }

            if (suchThatStmtRefStmtRef.find(clauseType) != suchThatStmtRefStmtRef.end()) {
                // Example: Follows(s1, s2)
                
                // Get all the stmt number of type second arg
                StatementType st2 = tokenTypeToStatementType[pq.declarations[rightArg.value]];
                set<shared_ptr<Statement>> allStmtOfRightArgDeclaration = servicer->getAllStmt(st2);
                vector<int> allStmtLinesOfRightArgDeclaration;

                for (shared_ptr<Statement> s : allStmtOfRightArgDeclaration) {
                    allStmtLinesOfRightArgDeclaration.push_back(s->getLineNum());
                }

                StmtStmtRelationType ss = tokenTypeToStmtStmtRelationType[clauseType];

                // Get statement lines of type s1 (if s1 is IF , we get all the IF statements)
                // for every line in the first arg s1, get ALL the statement s2 such that Follows(s1, s2) via forward Retrieve
                // from these statements s2, filter it down to the s2 type (if s2 is ASSIGN, we filter these s2 statements)
                // then return combinations of s1, s2 

                for (int line : allStmtLinesOfLeftArgDeclaration) {

                    vector<int> allStmtLinesWithLeftArg = servicer->forwardRetrieveRelation(line, ss);
                    vector<int> stmtLinesOfRightArg;

                    // This gives us all the s2 such that Follows(s1, s2) is true
                    getListOfStmtNumbersIntersection(stmtLinesOfRightArg, allStmtLinesWithLeftArg, allStmtLinesOfRightArgDeclaration);

                    for (int i : stmtLinesOfRightArg) {
                        intermediate.push_back(vector<string>
                        { to_string(line), to_string(i) });
                    }
                }
            }
            else {
                // Example: Uses(s, v) or Modifies(s, v);
                StmtVarRelationType sv = tokenTypeToStmtVarRelationType[clauseType];

                for (int line : allStmtLinesOfLeftArgDeclaration) {
                    vector<string> allVariables = servicer->forwardRetrieveRelation(line, sv);

                    for (string v : allVariables) {
                        intermediate.push_back(vector<string> { to_string(line), v });
                    }
                }
            }
        }
    }

    return 0;
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
        if (tokenTypeToStatementType.find(type) != tokenTypeToStatementType.end()) {
            StatementType stmtType = tokenTypeToStatementType[type];
            set<shared_ptr<Statement>> statements = servicer->getAllStmt(stmtType);
            
            for (shared_ptr<Statement> s : statements) {
                result.push_back(to_string(s->getLineNum()));
            }
        }
    }
}

inline bool QueryEvaluator::checkIfClauseNoExists() {
    return pq.patternClauses.empty() && pq.suchThatClauses.empty();
}

bool QueryEvaluator::checkIfSelectSynonymExistsInClause() {
    for (auto clause : pq.suchThatClauses) {
        if (pq.select == clause.left.value || pq.select == clause.right.value) {
            return true;
        }

        // If its a boolean such-that, we must check it
        if (clause.left.type != TokenType::SYNONYM && clause.right.type != TokenType::SYNONYM) {
            return true;
        }
    }

    for (auto clause : pq.patternClauses) {
        if (pq.select == clause.clauseType.value || 
            pq.select == clause.left.value || pq.select == clause.right.value) {
            return true;
        }
    }

    return false;
}

void QueryEvaluator::getListOfStmtNumbersIntersection(vector<int>& result, vector<int>& table1, vector<int>& table2) {
    sort(table1.begin(), table1.end());
    sort(table2.begin(), table2.end());

    set_intersection(table1.begin(), table1.end(),
        table2.begin(), table2.end(),
        back_inserter(result));
}
