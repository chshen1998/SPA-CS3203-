#include "QPS/QueryExtractor.h"
#include "QPS/QPS.h"

#include <vector>
#include <unordered_map>

#include "catch.hpp"
using namespace std;

extern vector<PqlToken> basic_tokens;

extern vector<PqlToken> missing_semicolon;

extern vector<PqlToken> missing_select;

extern vector<PqlToken> undeclared_select_parameter;

extern vector<PqlToken> addPatternClause(vector<PqlToken> tokens);

extern vector<PqlToken> addUsesClause(vector<PqlToken> tokens);

extern vector<PqlToken> addModifiesClause(vector<PqlToken> tokens);

extern vector<PqlToken> addParentClause(vector<PqlToken> tokens);

extern vector<PqlToken> addFollowsClause(vector<PqlToken> tokens);