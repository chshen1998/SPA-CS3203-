#include "CallLoopChecker.h"
#include "AST/Statement/CallStatement.h"
#include "AST/Statement/WhileStatement.h"
#include "AST/Statement/IfStatement.h"
#include "SP/InvalidSyntaxException.h"

bool CallLoopChecker::checkCallLoop(shared_ptr<SourceCode> AST) {
    checkSameName(AST->getProcedures());

    procedureCallMap.clear();
    for (shared_ptr<Procedure> p: AST->getProcedures()) {
        vector<string> visited;
        procedureCallMap.insert({p->getProcedureName(), visited});
        populateCallMap(p);
    }

    vector<string> visited = {};
    shared_ptr<vector<string>> overallVisited = make_shared<vector<string>>();

    for (auto it = procedureCallMap.begin(); it != procedureCallMap.end(); it++) {
        visited.clear();
        if (find(overallVisited->begin(), overallVisited->end(), it->first) == overallVisited->end()) {
            checkCallMap(procedureCallMap, visited, it->first, overallVisited);
        }
    }

    return false;
}

void CallLoopChecker::populateCallMap(shared_ptr<Procedure> procedure) {
    string procedureName = procedure->getProcedureName();
    populateCallMap(procedureName, procedure->getStatements());
}

void CallLoopChecker::populateCallMap(string caller, vector<shared_ptr<Statement>> statements) {
    for (shared_ptr<Statement> s: statements) {
        if (dynamic_pointer_cast<CallStatement>(s) != nullptr) {
            addMapping(caller, dynamic_pointer_cast<CallStatement>(s)->getProcedureName());
        } else if (dynamic_pointer_cast<WhileStatement>(s) != nullptr) {
            populateCallMap(caller, dynamic_pointer_cast<WhileStatement>(s)->getStatements());
        } else if (dynamic_pointer_cast<IfStatement>(s) != nullptr) {
            populateCallMap(caller, dynamic_pointer_cast<IfStatement>(s)->getThenStatements());
            populateCallMap(caller, dynamic_pointer_cast<IfStatement>(s)->getElseStatements());
        }
    }
}

void CallLoopChecker::addMapping(string caller, string callee) {
    vector<string> value;
    value = procedureCallMap.find(caller)->second;

    if (find(value.begin(), value.end(), callee) == value.end()) {
        procedureCallMap.erase(caller);
        value.push_back(callee);
    }

    procedureCallMap.insert({caller, value});
}

void CallLoopChecker::checkSameName(vector<shared_ptr<Procedure>> procedures) {
    vector<string> procedureNames;
    for (shared_ptr<Procedure> p: procedures) {
        string name = p->getProcedureName();
        if (find(procedureNames.begin(), procedureNames.end(), name) == procedureNames.end()) {
            procedureNames.push_back(name);
        } else {
            throw InvalidSyntaxException((char *) "2 Procedures cannot have the same name");
        }
    }
}

void CallLoopChecker::checkCallMap(
        map<string, vector<string>> myMap,
        vector<string> visited,
        string currNode,
        shared_ptr<vector<string>> overallVisited) {
    // If currNode has been visited before
    if (find(visited.begin(), visited.end(), currNode) != visited.end()) {
        throw InvalidSyntaxException((char *) "Recursive procedure call loops are not allowed");
    }

    if (myMap.find(currNode) == myMap.end()) {
        throw InvalidSyntaxException((char *) "Cannot call a procedure that does not exist");
    }

    visited.push_back(currNode);
    overallVisited->push_back(currNode);

    string key = currNode;
    vector<string> value = myMap.find(currNode)->second;

    myMap.erase(key);
    for (string s: value) {
        checkCallMap(myMap, visited, s, overallVisited);
    }

}
