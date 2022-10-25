#include <iso646.h>

using namespace std;

#include <string>
#include <vector>
#include <set>

#include "QueryOptimizer.h"
#include "./Structures/PqlError.h"
#include "./Structures/PqlToken.h"
#include "./Structures/PqlQuery.h"
#include "./Types/ErrorType.h"
#include "./Types/TokenType.h"
#include "Validators/ValidatorUtils.h"

QueryOptimizer::QueryOptimizer(PqlQuery& queryObj) 
{
	pq = queryObj;
    synonymSets = {};
    clauses = pq.clauses[0];
}

void QueryOptimizer::optimize() 
{
    pq.clauses.clear();

    groupClauses();

    sortGroupOrder();

    sortGroupClauses();
}

/*  Divide the clauses into multiple groups
*   1. Clauses without synonyms (Boolean clauses) should be 1 group
*   2. Clauses with common synonyms should be in same group
*/
void QueryOptimizer::groupClauses() 
{
    // Divide clauses into separate groups
    for (Clause clause : clauses) {
        // 1. Boolean clauses (Clauses without synonyms)
        if (clause.checkIfBooleanClause()) {
            pq.booleanClauses.push_back(clause);
            continue;
        }

        // 2. Clauses with connected synonyms should be in the same group
        int index = 0;
        for (set<string> synonymSet : synonymSets) {
            if (clause.left.type == TokenType::SYNONYM && synonymSet.find(clause.left.value) != synonymSet.end()) {
                break;
            }
            if (clause.right.type == TokenType::SYNONYM && synonymSet.find(clause.right.value) != synonymSet.end()) {
                break;
            }
            index++;
        }

        if (index == pq.clauses.size()) {
            pq.clauses.push_back(vector<Clause>{});
            synonymSets.push_back(set<string>{});

        }

        pq.clauses[index].push_back(clause);
        if (clause.left.type == TokenType::SYNONYM) {
            synonymSets[index].insert(clause.left.value);
        }
        if (clause.right.type == TokenType::SYNONYM) {
            synonymSets[index].insert(clause.right.value);
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
    for (SelectObject obj : pq.selectObjects) {
        if (obj.type != SelectType::BOOLEAN) {
            selectSynonyms.push_back(obj.synonym);
        }
    }

    int left = 0;
    int right = pq.clauses.size();
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
            vector<Clause> temp = pq.clauses[left];
            pq.clauses[left] = pq.clauses[right];
            pq.clauses[right] = temp;
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
    for (vector<Clause> group : pq.clauses) {

    }
}