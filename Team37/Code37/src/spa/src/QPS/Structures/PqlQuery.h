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
struct Clause
{
    PqlToken clauseType;
    PqlToken left;
    PqlToken right;

    Clause(PqlToken type, PqlToken l, PqlToken r) : clauseType(type), left(l), right(r) {}

    bool operator==(const Clause& other) const {
        return (other.clauseType == clauseType) && (other.left == left) && (other.right == right);
    }
};

/*
 * ParsedQueries are created after extracting the components from the query.
 */
struct PqlQuery {
    unordered_map<string, TokenType> declarations = {};
    string select;
    vector<Clause> patternClauses;
    vector<Clause> suchThatClauses;
};

#endif 