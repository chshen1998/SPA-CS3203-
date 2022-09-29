using namespace std;

#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>

#include "../Types/TokenType.h"
#include "PqlToken.h"
#include "PqlQuery.h"

Clause::Clause(PqlToken type, PqlToken l, PqlToken r, TokenType categoryType)
{
    clauseType = type;
    left = l;
    right = r;
    category = categoryType;
}

PatternClause::PatternClause(PqlToken type, PqlToken l, PqlToken r) : Clause(type, l, r, TokenType::PATTERN) {}

WithClause::WithClause(PqlToken type, PqlToken l, PqlToken r, PqlToken la, PqlToken ra) : Clause(type, l, r, TokenType::WITH)
{
    leftAttr = la;
    rightAttr = ra;
}

SuchThatClause::SuchThatClause(PqlToken type, PqlToken l, PqlToken r) : Clause(type, l, r, TokenType::SUCH_THAT) {}