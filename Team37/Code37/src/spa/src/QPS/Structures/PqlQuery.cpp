using namespace std;

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "../Types/TokenType.h"
#include "PqlQuery.h"
#include "PqlToken.h"

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

SelectObject::SelectObject(SelectType t)
{
    type = t;
    synonym = "";
    attrName = PqlToken();
}

SelectObject::SelectObject(SelectType t, string s)
{
    type = t;
    synonym = s;
    attrName = PqlToken();
}

SelectObject::SelectObject(SelectType t, string s, PqlToken a)
{
    type = t;
    synonym = s;
    attrName = a;
}
