using namespace std;

#include <string>
#include <unordered_map>
#include <vector>

#include "../Types/TokenType.h"

#ifndef TEAM37_PQLQUERY_H
#define TEAM37_PQLQUERY_H

/*
 * PatternClause has 2 synonyms, the left synonym appears on the LHS of the assignment while the right synonym
 * appears on the RHS of the assignment.
 *
 * These synonyms must be variable type.
 */
class Clause
{
public:
    Clause(PqlToken clauseType, PqlToken left, PqlToken right, TokenType category);

    Clause(PqlToken clauseType, PqlToken left, PqlToken right, TokenType category, PqlToken leftAttr, PqlToken rightAttr);

    bool operator==(const Clause& other) const {
        if (category == TokenType::WITH)
        {
	        return (other.clauseType == clauseType) && (other.left == left) && (other.right == right) && (other.leftAttr == leftAttr) && (other.rightAttr == rightAttr);
        }
        return (other.clauseType == clauseType) && (other.left == left) && (other.right == right);
    }

    PqlToken clauseType;
    PqlToken left;
    PqlToken leftAttr;
    PqlToken right;
    PqlToken rightAttr;
    TokenType category;
};

/*
 * ParsedQueries are created after extracting the components from the query.
 */
struct PqlQuery {
    unordered_map<string, TokenType> declarations = {};
    string select;
    vector<Clause> clauses;
    vector<Clause> patternClauses;
    vector<Clause> suchThatClauses;
};

#endif 