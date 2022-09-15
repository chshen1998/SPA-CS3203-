#include "QPS/QueryExtractor.h"
#include "QPS/QPS.h"

#include <vector>
#include <unordered_map>

#include "catch.hpp"
using namespace std;

extern vector<PqlToken> basic_tokens;

extern vector<PqlToken> multi_declarations;

extern vector<PqlToken> missing_semicolon;

extern vector<PqlToken> missing_select;

extern vector<PqlToken> undeclared_select_parameter;

extern vector<PqlToken> valid_pattern;

extern vector<PqlToken> undeclared_pattern_assign;

extern vector<PqlToken> undeclared_pattern_parameter;

extern vector<PqlToken> invalid_pattern_parameter;

extern vector<PqlToken> pattern_missing_open_bracket;

extern vector<PqlToken> addPatternClause(vector<PqlToken> tokens);

extern vector<PqlToken> addUsesClause(vector<PqlToken> tokens);

extern vector<PqlToken> addModifiesClause(vector<PqlToken> tokens);

extern vector<PqlToken> addParentClause(vector<PqlToken> tokens);

extern vector<PqlToken> addFollowsClause(vector<PqlToken> tokens);