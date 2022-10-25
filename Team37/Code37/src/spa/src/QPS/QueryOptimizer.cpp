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
        if (pq->clauses[0][i].checkIfBooleanClause()) {
            pq->booleanClauses.push_back(pq->clauses[0][i]);
            pq->clauses[0].erase(pq->clauses[0].begin() + i);
            size--;
            continue;
        }
        
        // 2. Clauses with connected synonyms should be in the same group
        int setNum = 0;
        for (set<string> synonymSet : synonymSets) {
            if (pq->clauses[0][i].left.type == TokenType::SYNONYM && synonymSet.find(pq->clauses[0][i].left.value) != synonymSet.end() ||
                pq->clauses[0][i].right.type == TokenType::SYNONYM && synonymSet.find(pq->clauses[0][i].right.value) != synonymSet.end()) {
                break;
            }
            setNum++;
        }

        if (setNum == pq->clauses.size()) {
            pq->clauses.push_back(vector<Clause>{});
            synonymSets.push_back(set<string>{});
        }

        if (pq->clauses[0][i].left.type == TokenType::SYNONYM) {
            synonymSets[setNum].insert(pq->clauses[0][i].left.value);
        }

        if (pq->clauses[0][i].right.type == TokenType::SYNONYM) {
            synonymSets[setNum].insert(pq->clauses[0][i].right.value);
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
    for (SelectObject obj : pq->selectObjects) {
        if (obj.type != SelectType::BOOLEAN) {
            selectSynonyms.push_back(obj.synonym);
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
*   2. Prioritize clauses with less number of results: Follows, Modifies, etc.
*   3. Prioritize with-clauses – more restrictive than such that clauses
*   4. Push Affects clauses and *-clauses to the last positions in a group
*/
void QueryOptimizer::sortGroupClauses() {
    for (vector<Clause> group : pq->clauses) {
        int left = 0;
        int right = group.size() - 1;
        while (left < right) {
            if (designAbstractionsA.find(group[left].clauseType.type) != designAbstractionsA.end()) {
                swap(group[left], group[right]);
                right--;
            }
            else {
                left++;
            }
        }
    }
}