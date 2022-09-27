#include "QPS/QueryExtractor.h"
#include "QPS/Structures/PqlError.h"
#include "QPS/Structures/PqlToken.h"
#include "QPS/Structures/PqlQuery.h"
#include "QPS/Types/ErrorType.h"
#include "QPS/Types/TokenType.h"

#include <vector>
#include <unordered_map>

#include "catch.hpp"
using namespace std;

extern vector<PqlToken> basic_tokens;

extern vector<PqlToken> valid_pattern;

extern vector<PqlToken> valid_uses;

extern vector<PqlToken> valid_modifies;

extern vector<PqlToken> valid_follows;

extern vector<PqlToken> valid_parent;

extern vector<PqlToken> valid_follows_a;

extern vector<PqlToken> valid_parent_a;

extern vector<PqlToken> valid_follows_a;

extern vector<PqlToken> valid_parent_a;

extern vector<PqlToken> valid_such_that_then_pattern;

extern vector<PqlToken> valid_pattern_then_such_that;

extern vector<PqlToken> multi_declarations;

extern vector<PqlToken> missing_semicolon;

extern vector<PqlToken> missing_select;

extern vector<PqlToken> undeclared_select_parameter;

extern vector<PqlToken> undeclared_pattern_assign;

extern vector<PqlToken> undeclared_pattern_parameter;

extern vector<PqlToken> invalid_pattern_parameter;

extern vector<PqlToken> pattern_missing_open_bracket;

extern vector<PqlToken> missing_such_that;

extern vector<PqlToken> invalid_wildcard_uses;

extern vector<PqlToken> invalid_wildcard_modifies;

extern vector<PqlToken> valid_multi_pattern_then_multi_such_that;

extern vector<PqlToken> addPatternClause(vector<PqlToken> tokens);

extern vector<PqlToken> addUsesClause(vector<PqlToken> tokens);

extern vector<PqlToken> addModifiesClause(vector<PqlToken> tokens);

extern vector<PqlToken> addParentClause(vector<PqlToken> tokens);

extern vector<PqlToken> addFollowsClause(vector<PqlToken> tokens);