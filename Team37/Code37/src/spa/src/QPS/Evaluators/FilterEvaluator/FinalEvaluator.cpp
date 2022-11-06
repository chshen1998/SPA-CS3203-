using namespace std;

#include <unordered_set>
#include <iostream>
#include <list>

#include "QPS/Structures/PqlQuery.h"
#include "QPS/Structures/PqlToken.h"
#include "QPS/Types/TokenType.h"
#include "FinalEvaluator.h"
#include "PKB/Types/StatementType.h"
#include "PKB/Types/StmtVarRelationType.h"
#include "QPS/Evaluators/EvaluatorUtils.h"

using namespace EvaluatorUtils;

void FinalEvaluator::getFinalResult(list<string> &result, vector<vector<string>> &intermediate) {
    // If there are no clauses (our table will not even have column headers) OR
    // If all of our select synonyms not appearing in clause
    if (intermediate.size() == 0 || !checkIfClauseContainsSelect()) {
        getAllCombinations(result);
        return;
    }

    // If there are any call/print/read synonyms with no ProcName/VarName but is required
    // in SelectObjects, we add them to our intermediate table
    updateFinalTableWithAttrName(intermediate);

    // Get the mapping of indexes from the intermediate table to the Select Object
    unordered_map<int, int> tableIndexToSelectIndex = getTableIndexToSelectIndex(intermediate);

    // Initialize our current row
    vector<string> currentRow(pq->selectObjects.size(), "");
    // Just to prevent duplicates in our final result
    unordered_set<string> uniqueResults;

    // Add the result values of that column into result
    for (int j = 1; j < intermediate.size(); j++) {
        for (int k = 0; k < intermediate[j].size(); k++) {
            if (tableIndexToSelectIndex.find(k) != tableIndexToSelectIndex.end()) {
                currentRow[tableIndexToSelectIndex[k]] = intermediate[j][k];
            }
        }

        string row = "";
        for (int index = 0; index < currentRow.size(); index++) {
            row += currentRow[index];

            if (index != currentRow.size() - 1) {
                row += " ";
            }
        }

        if (uniqueResults.find(row) == uniqueResults.end()) {
            result.push_back(row);
            uniqueResults.insert(row);
        }
    }

    return;
}


unordered_map<int, int> FinalEvaluator::getTableIndexToSelectIndex(vector<vector<string>> &intermediate) {
    unordered_map<string, int> selectSynonymToIndex;
    unordered_map<int, int> tableIndexToSelectIndex;

    // Get the index of all the tuple elems
    for (int i = 0; i < pq->selectObjects.size(); i++) {
        string name = pq->selectObjects[i]->synonym;

        if (checkIfSelectSynonymIsDoubleAttr(pq->selectObjects[i])) {
            name += "." + pq->selectObjects[i]->attrName.value;
        }

        selectSynonymToIndex.insert({name, i});
    }

    // Get the mapping of select synonym index in intermediate table to index in tuple
    for (int j = 0; j < intermediate[0].size(); j++) {
        if (selectSynonymToIndex.find(intermediate[0][j]) != selectSynonymToIndex.end()) {
            tableIndexToSelectIndex.insert({j, selectSynonymToIndex[intermediate[0][j]]});
        }
    }

    return tableIndexToSelectIndex;
}


void FinalEvaluator::updateFinalTableWithAttrName(vector<vector<string>> &intermediate) {
    // In the Select Objects, We get all the:
    // Call AttrName types with ProcName
    // Read / Print AttrName types with VarName
    unordered_set<string> callReadPrintWithAltAttrName;
    for (shared_ptr<SelectObject> s: pq->selectObjects) {
        if (checkIfSelectSynonymIsDoubleAttr(s)) {
            callReadPrintWithAltAttrName.insert(s->synonym);
        }
    }

    // In our intermediate table, we get all the call/print/read synonyms that 
    // have not been enhanced with ProcName / VarName respectively
    unordered_set<string> callPrintReadSynonym;
    for (int i = 0; i < intermediate[0].size(); i++) {
        if (doubleAttrTokens.find(declarations[intermediate[0][i]]) != doubleAttrTokens.end()) {
            callPrintReadSynonym.insert(intermediate[0][i]);
            continue;
        }

        // ".procName"
        size_t dotPosition = intermediate[0][i].find(".procName");

        if (dotPosition == -1) {
            // ".varName"
            dotPosition = intermediate[0][i].find(".varName");

            if (dotPosition == -1) {
                continue;
            }
        }

        string synonym = intermediate[0][i].substr(0, dotPosition);
        callPrintReadSynonym.erase(synonym);
    }

    // Add .ProcName or VarName to call/print/read synonyms if they are part of a select synonym
    for (string s: callPrintReadSynonym) {
        if (callReadPrintWithAltAttrName.find(s) != callReadPrintWithAltAttrName.end()) {
            addAttrName(intermediate, PqlToken(TokenType::SYNONYM, s));
        }
    }
    // With this we should have the final proper intermediate table to pull the selectObjects
}


bool FinalEvaluator::checkIfClauseContainsSelect() {
    if (pq->selectObjects[0]->type == SelectType::BOOLEAN) {
        return false;
    }

    unordered_set<string> selectElements;

    for (shared_ptr<SelectObject> s: pq->selectObjects) {
        selectElements.insert(s->synonym);
    }

    for (shared_ptr<Clause> c: pq->clauses[0]) {
        if (selectElements.find(c->left.value) != selectElements.end() ||
            selectElements.find(c->right.value) != selectElements.end() ||
            selectElements.find(c->clauseType.value) != selectElements.end()) {
            return true;
        }
    }

    return false;
}


// In the case where we have no clauses, we get all combinations for tuples
void FinalEvaluator::getAllCombinations(list<string> &result) {
    vector<vector<string>> finalResult;

    for (shared_ptr<SelectObject> s: pq->selectObjects) {
        TokenType declarationType = pq->declarations[s->synonym];

        if (s->type == SelectType::ATTRNAME &&
            attrNameToTokenType.find(s->attrName.type) != attrNameToTokenType.end()) {
            declarationType = attrNameToTokenType[s->attrName.type];
        }

        finalResult.push_back(selectAll(declarationType));
    }

    vector<int> indexes;
    indexes.reserve(finalResult.size());

    for (int i = 0; i < finalResult.size(); i++) {
        indexes.push_back(0);
    }

    while (true) {
        // We add combine our row into a string, and then add it to result
        string row = "";
        for (int k = 0; k < finalResult.size(); k++) {
            row += finalResult[k][indexes[k]];

            if (k != finalResult.size() - 1) {
                row += " ";
            }
        }
        result.push_back(row);

        int current = 0;
        indexes[current] += 1;

        while (indexes[current] == finalResult[current].size()) {
            if (current == finalResult.size() - 1) {
                return;
            }

            indexes[current] = 0;
            current += 1;
            indexes[current] += 1;
        }
    }
}

bool FinalEvaluator::checkIfSelectSynonymIsDoubleAttr(shared_ptr<SelectObject> s) {
    if (s->type == SelectType::ATTRNAME) {
        TokenType synonymType = pq->declarations[s->synonym];

        return ((synonymType == TokenType::CALL && s->attrName.type == TokenType::PROCNAME) ||
                ((synonymType == TokenType::READ || synonymType == TokenType::PRINT) &&
                 s->attrName.type == TokenType::VARNAME));
    }

    return false;
}