#include "QueryServicer.h"

/*
* Constructor
* @param Storage Storage Object
*/
QueryServicer::QueryServicer(shared_ptr<Storage> storage) {
    this->storage = storage;
}

/*
* Getter for all Variables
* @return Set of all NameExpression Nodes
*/
set<NameExpression> QueryServicer::getAllVar() {
    return storage->getAllVar();
}

/*
* Getter for all constatns
* @return Set of all ConstantExpression Nodes
*/
set<ConstantExpression> QueryServicer::getAllConst() {
    return storage->getAllConst();
}

/*
* Getter for all constatns
* @return Set of all ConstantExpression Nodes
*/
set<Procedure> QueryServicer::getAllProc() {
    return storage->getAllProc();
}

/*
* Get all statement or statement subtypes
* @param type Statement type
* @return Set of pointers to statements
*/
set<shared_ptr<Statement>> QueryServicer::getAllStmt(StatementType type) {
    set<shared_ptr<Statement>> allStmt = storage->getAllStmt();
    set<shared_ptr<Statement>> filteredStmt = {};
    set<shared_ptr<Statement>>::iterator itr;
    switch (type) {
        case STATEMENT:
            return allStmt;
            break;
        case ASSIGN:
            copy_if(allStmt.begin(), allStmt.end(), inserter(filteredStmt, filteredStmt.end()),
                    [](shared_ptr<Statement> ptr) {
                        return dynamic_pointer_cast<AssignStatement>(ptr) != nullptr;
                    });

            return filteredStmt;
            break;
        case CALL:
            copy_if(allStmt.begin(), allStmt.end(), inserter(filteredStmt, filteredStmt.end()),
                    [](shared_ptr<Statement> ptr) {
                        return dynamic_pointer_cast<CallStatement>(ptr) != nullptr;
                    });
            return filteredStmt;
            break;
        case IF:
            copy_if(allStmt.begin(), allStmt.end(), inserter(filteredStmt, filteredStmt.end()),
                    [](shared_ptr<Statement> ptr) {
                        return dynamic_pointer_cast<IfStatement>(ptr) != nullptr;
                    });
            return filteredStmt;
            break;
        case PRINT:
            copy_if(allStmt.begin(), allStmt.end(), inserter(filteredStmt, filteredStmt.end()),
                    [](shared_ptr<Statement> ptr) {
                        return dynamic_pointer_cast<PrintStatement>(ptr) != nullptr;
                    });
            return filteredStmt;
            break;
        case READ:
            copy_if(allStmt.begin(), allStmt.end(), inserter(filteredStmt, filteredStmt.end()),
                    [](shared_ptr<Statement> ptr) {
                        return dynamic_pointer_cast<ReadStatement>(ptr) != nullptr;
                    });

            return filteredStmt;
            break;
        case WHILE:
            copy_if(allStmt.begin(), allStmt.end(), inserter(filteredStmt, filteredStmt.end()),
                    [](shared_ptr<Statement> ptr) {
                        return dynamic_pointer_cast<WhileStatement>(ptr) != nullptr;
                    });
            return filteredStmt;
            break;
        default:
            throw invalid_argument("No Statement Type Specified");
            break;
    }
}

/*
Retrieve Relation Stored. For Relation(stmt1, stmt2)
@param stmt1
@param stmt2
@param type Type of relation
@returns If Relation(stmt1, stmt2) is True
*/
bool QueryServicer::retrieveRelation(int stmt1, int stmt2, StmtStmtRelationType type) {
    return storage->retrieveRelation(stmt1, stmt2, type);
}

/*
Retrieve Forward Relation Stored. For Relation(stmt1, stmt2)
@param stmt1
@param type Type of relation
@returns All stmt2 such that Relatioin(stmt1, stmt2) is True
*/
vector<int> QueryServicer::forwardRetrieveRelation(int stmt1, StmtStmtRelationType type) {
    return storage->forwardRetrieveRelation(stmt1, type);
}

/*
Retrieve Reverse Relation Stored. For Relation(stmt1, stmt2)
@param stmt2
@param type Type of relation
@returns All stmt1 such that Relation(stmt1, stmt2) is True
*/
vector<int> QueryServicer::reverseRetrieveRelation(int stmt2, StmtStmtRelationType type) {
    return storage->reverseRetrieveRelation(stmt2, type);
}

/*
Retrieve Relation Stored. For Relation(stmt, var)
@param stmt Statement Number
@param var Variable Name
@param type Type of relation
@returns If Relation(stmt, var) is True
*/
bool QueryServicer::retrieveRelation(int stmt, string var, StmtVarRelationType type) {
    return storage->retrieveRelation(stmt, var, type);
}

/*
Retrieve Forward Relation Stored. For Relation(stmt, var)
@param stmt
@param type Type of relation
@returns All var such that Relatioin(stmt, var) is True
*/
vector<string> QueryServicer::forwardRetrieveRelation(int stmt, StmtVarRelationType type) {
    return storage->forwardRetrieveRelation(stmt, type);
}

/*
Retrieve Reverse Relation Stored. For Relation(stmt, var)
@param var
@param type Type of relation
@returns All stmt1 such that Relatioin(stmt, var) is True
*/
vector<int> QueryServicer::reverseRetrieveRelation(string var, StmtVarRelationType type) {
    return storage->reverseRetrieveRelation(var, type);
}

/*
Retrieve Relation Stored. For Relation(proc, var)
@param proc procedure name
@param var Variable Name
@param type Type of relation
@returns If Relation(proc, var) is True
*/
bool QueryServicer::retrieveRelation(string proc, string var, ProcVarRelationType type) {
    return storage->retrieveRelation(proc, var, type);
}

/*
Retrieve Forward Relation Stored. For Relation(proc, var)
@param proc procedure name
@param type Type of relation
@returns All var such that Relatioin(proc, var) is True
*/
vector<string> QueryServicer::forwardRetrieveRelation(string proc, ProcVarRelationType type) {
    return storage->forwardRetrieveRelation(proc, type);
}

/*
Retrieve Reverse Relation Stored. For Relation(proc, var)
@param var
@param type Type of relation
@returns All stmt1 such that Relatioin(proc, var) is True
*/
vector<string> QueryServicer::reverseRetrieveRelation(string var, ProcVarRelationType type) {
    return storage->reverseRetrieveRelation(var, type);
}

/*
Retrieve Reverse assignment statement stored. For Pattern(_,"x") and Pattern(_,_"x"_)
@param queryString string to match
@param hasWildcard whether query contains wildcard
@returns All assign stmt a such that pattern a (_,queryString) or (_,_queryString_) returns true
*/
set<int> QueryServicer::reverseRetrievePatternMatch(const string &queryString, bool hasWildcard) {

    set<int> matchingLineNum = {};
    for (auto stmt: storage->getAllStmt()) {
        if (dynamic_pointer_cast<AssignStatement>(stmt) != nullptr) {
            shared_ptr<AssignStatement> assignStmt = dynamic_pointer_cast<AssignStatement>(stmt);
            int currLineNum = assignStmt->getLineNum();
            string generatedString = assignStmt->getRelFactor()->generateString();
            // if (_,_queryString_) we do substring match
            if (hasWildcard) {
                deque<string> parsedRelationalFactors = parseRelationalFactorString(generatedString);
                for (const string &parsedFactor: parsedRelationalFactors) {
                    if (sanitizeString(parsedFactor) == queryString) {
                        matchingLineNum.insert(currLineNum);
                    }
                }
                // if (_,queryString) we do exact string match
            } else {
                if (sanitizeString(generatedString) == queryString) {
                    matchingLineNum.insert(currLineNum);
                }
            }
        }
    }
    return matchingLineNum;
}

// helpers classes for pattern matching

/**
 * Parses relational factor string into its bracketed substrings
 * @param str
 * @return deque of bracketed substrings
 */
deque<string> QueryServicer::parseRelationalFactorString(const std::string &str) {
    deque<std::string> result;
    stack<std::string::const_iterator> stack;
    for (auto it = str.begin(); it != str.end();) {
        if (*it == '(') {
            stack.push(it++);
        } else if (*it == ')') {
            auto start = stack.top();
            stack.pop();
            result.emplace_back(start, ++it);
        } else {
            it++;
        }
    }
    return result;
}

/**
 * Remove brackets from  string
 * @param word
 * @return sanitized word
 */
string QueryServicer::sanitizeString(string word) {
    int i = 0;

    while (i < word.size()) {
        if (word[i] == '(' || word[i] == ')') {
            word.erase(i, 1);
        } else {
            i++;
        }
    }
    return word;
}

/*
Retrieve Relation Stored. For Relation(proc1, proc2)
@param proc1
@param proc2
@param type Type of relation
@returns If Relation(proc1, proc2) is True
*/
bool QueryServicer::retrieveRelation(string proc1, string proc2, ProcProcRelationType type) {
    return storage->retrieveRelation(proc1, proc2, type);
}

/*
Retrieve Forward Relation Stored. For Relation(proc1, proc2)
@param proc1
@param type Type of relation
@returns All var such that Relation(proc1, proc2) is True
*/
vector<string> QueryServicer::forwardRetrieveRelation(string proc1, ProcProcRelationType type) {
    return storage->forwardRetrieveRelation(proc1, type);
}

/*
Retrieve Reverse Relation Stored. For Relation(proc1, proc2)
@param proc2
@param type Type of relation
@returns All stmt1 such that Relation(proc1, proc2) is True
*/
vector<string> QueryServicer::reverseRetrieveRelation(string proc2, ProcProcRelationType type) {
    return storage->reverseRetrieveRelation(proc2, type);
}

vector<int> QueryServicer::forwardComputeRelation(int stmt, StmtStmtRelationType type) {
    return storage->forwardComputeRelation(stmt, type);
}

vector<int> QueryServicer::backwardComputeRelation(int stmt, StmtStmtRelationType type) {
    return storage->backwardComputeRelation(stmt, type);
}