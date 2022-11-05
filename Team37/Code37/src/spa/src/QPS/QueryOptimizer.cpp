#include <iso646.h>

using namespace std;

#include <string>
#include <vector>
#include <set>
#include <memory>

#include "QueryOptimizer.h"
#include "./Structures/PqlError.h"
#include "./Structures/PqlToken.h"
#include "./Structures/PqlQuery.h"
#include "./Types/ErrorType.h"
#include "./Types/TokenType.h"
#include "Validators/ValidatorUtils.h"

set<TokenType> designAbstractionsA = {
    TokenType::AFFECTS,
    TokenType::AFFECTS_A,
    TokenType::NEXT_A,
    TokenType::CALLS_A,
    TokenType::FOLLOWS_A,
    TokenType::PARENT_A
};

QueryOptimizer::QueryOptimizer(shared_ptr<PqlQuery> pq_pointer)
{
	pq = pq_pointer;
    synonymSets = {};
}

void QueryOptimizer::optimize() 
{
    groupClauses();

    // If no synonym clauses then optimization is complete
    if (pq->clauses[0].size() == 0) {
        pq->clauses.clear();
        return;
    }

    sortGroupOrder();

    //sortGroupClauses();
}

/*  Divide the clauses into multiple groups
*   1. Clauses without synonyms (Boolean clauses) should be 1 group
*   2. Clauses with common synonyms should be in same group
*/
void QueryOptimizer::groupClauses() 
{
    synonymSets.push_back({});

    // Divide clauses into separate groups
    int i = 0;
    int size = pq->clauses[0].size();
    while (i < size) {
        // 1. Boolean clauses (Clauses without synonyms)
        if (pq->clauses[0][i]->checkIfBooleanClause()) {
            pq->booleanClauses.push_back(pq->clauses[0][i]);
            pq->clauses[0].erase(pq->clauses[0].begin() + i);
            size--;
            continue;
        }
        
        // 2. Clauses with connected synonyms should be in the same group
        int setNum = 0;
        for (set<string> synonymSet : synonymSets) {
            if (pq->clauses[0][i]->left.type == TokenType::SYNONYM && synonymSet.find(pq->clauses[0][i]->left.value) != synonymSet.end() ||
                pq->clauses[0][i]->right.type == TokenType::SYNONYM && synonymSet.find(pq->clauses[0][i]->right.value) != synonymSet.end()) {
                break;
            }
            setNum++;
        }

        if (setNum == pq->clauses.size()) {
            pq->clauses.push_back(vector<shared_ptr<Clause>>{});
            synonymSets.push_back(set<string>{});
        }

        if (pq->clauses[0][i]->left.type == TokenType::SYNONYM) {
            synonymSets[setNum].insert(pq->clauses[0][i]->left.value);
        }

        if (pq->clauses[0][i]->right.type == TokenType::SYNONYM) {
            synonymSets[setNum].insert(pq->clauses[0][i]->right.value);
        }

        if (setNum == 0) {
            i++;
        } else {
            pq->clauses[setNum].push_back(pq->clauses[0][i]);
            size--;
        }
    }
}


/*  Sort groups for evaluation
*   1. Start with clauses without synonyms (Boolean clauses)
*   2. Prioritize groups that do not invlove synonyms in Select to be evaluated first
*/
void QueryOptimizer::sortGroupOrder()
{
    vector<string> selectSynonyms;
    for (shared_ptr<SelectObject> obj : pq->selectObjects) {
        if (obj->type != SelectType::BOOLEAN) {
            selectSynonyms.push_back(obj->synonym);
        }
    }

    int left = 0;
    int right = pq->clauses.size()-1;
    while (left < right) {
        bool containsSelect = false;
        for (string synonym : selectSynonyms) {
            // If clause group involves select synonyms then move it to the back
            if (synonymSets[left].find(synonym) != synonymSets[left].end()) {
                containsSelect = true;
                break;
            }
        }
        
        if (containsSelect) {
            swap(pq->clauses[left], pq->clauses[right]);
            right--;
        }
        else {
            left++;
        }
    }
}


/*  Sort clauses inside each group 
*   1. Prioritize clauses with one constant and one synonym
*   2. Prioritize with-clauses – more restrictive than such that clauses
*   3. Push Affects clauses and *-clauses to the last positions in a group
*/
void QueryOptimizer::sortGroupClauses() {
    for (vector<shared_ptr<Clause>> group : pq->clauses) {
        vector<int> priorityScores(group.size());
        int currScore;

        for (int i = 0; i < group.size(); i++) {
            currScore = 0;

            // Mark Affects and *-clauses with priority of -1
            if (designAbstractionsA.find(group[i]->clauseType.type) != designAbstractionsA.end()) {
                priorityScores[i] = -1;
                continue;
            }

            // Increase priority if clause contains a constant
            if (group[i]->left.type != TokenType::SYNONYM || group[i]->right.type != TokenType::SYNONYM) {
                priorityScores[i]++;
            }

            // Increase priority if clauses 
            if (group[i]->category == TokenType::WITH) {
                priorityScores[i]++;
            }
        }

        quickSort(&group, &priorityScores, 0, group.size() - 1);
    }
}

void QueryOptimizer::quickSort(vector<shared_ptr<Clause>>* group, vector<int>* scores, int left, int right) {
    if (left < right) {
        int p = partition(group, scores, left, right);
        quickSort(group, scores, left, p - 1);
        quickSort(group, scores, right, p + 1);
    }
}

int QueryOptimizer::partition(vector<shared_ptr<Clause>>* group, vector<int>* scores, int left, int right) {
    int pivot = scores->at(right);
    int i = left - 1;
    for (int j = left; j < right; j++) {
        if (scores->at(j) <= pivot) {
            i += 1;
            swap(scores[i], scores[j]);
            swap(group[i], group[j]);
        }
    }
    swap(scores[i + 1], scores[right]);
    swap(group[i + 1], group[right]);
    return i + 1;
}