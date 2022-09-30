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

Clause::Clause(PqlToken type, PqlToken l, PqlToken r, TokenType categoryType, PqlToken la, PqlToken ra)
{
    clauseType = type;
    left = l;
    leftAttr = la;
    right = r;
    rightAttr = ra;
    category = categoryType;
}