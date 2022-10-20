using namespace std;

#include <string>
#include <unordered_map>
#include <vector>

#include "../Types/TokenType.h"
#include "PqlToken.h"

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
    // Uses (s, v)
    // with s.varname = "x"
    PqlToken clauseType;
    PqlToken left;
    PqlToken leftAttr;
    PqlToken right;
    PqlToken rightAttr;
    TokenType category;

    inline bool checkIfBooleanClause() {
        return left.type != TokenType::SYNONYM && right.type != TokenType::SYNONYM;
    };
};

enum class SelectType {
    BOOLEAN,
    SYNONYM,
    ATTRNAME
};

class SelectObject
{
public:
    SelectObject(SelectType type);

    SelectObject(SelectType type, string synonym);

    SelectObject(SelectType type, string synonym, PqlToken attrName);
    
    SelectType type;
    string synonym;
    PqlToken attrName;

    bool operator==(const SelectObject& other) const {
        return (other.type == type) && (other.synonym == synonym) && (other.attrName == attrName);
    }
};

/*
 * ParsedQueries are created after extracting the components from the query.
 */
struct PqlQuery {
    unordered_map<string, TokenType> declarations = {};
    vector<SelectObject> selectObjects;
    vector<Clause> booleanClauses;
    vector<vector<Clause>> clauses;
};

#endif 