#include "QPS/QueryExtractor.h"
#include "QPS/Structures/PqlError.h"
#include "QPS/Structures/PqlQuery.h"
#include "QPS/Structures/PqlToken.h"
#include "QPS/Types/ErrorType.h"
#include "QPS/Types/TokenType.h"

#include <unordered_map>
#include <vector>

#include "catch.hpp"
using namespace std;

extern vector<PqlToken> basic_tokens;

extern vector<PqlToken> valid_select_only;

extern vector<PqlToken> valid_calls_wildcards;

extern vector<PqlToken> valid_select_boolean;

extern vector<PqlToken> valid_select_declared_boolean;

extern vector<PqlToken> valid_select_declared_boolean_attrname;

extern vector<PqlToken> valid_select_attrname;

extern vector<PqlToken> valid_select_tuple;

extern vector<PqlToken> invalid_select_missing_arrows;

extern vector<PqlToken> invalid_select_missing_comma;

extern vector<PqlToken> valid_select_boolean;

extern vector<PqlToken> valid_pattern_assign;

extern vector<PqlToken> valid_pattern_assign_boolean;

extern vector<PqlToken> valid_pattern_while;

extern vector<PqlToken> valid_pattern_if;

extern vector<PqlToken> valid_pattern_multi;

extern vector<PqlToken> valid_pattern_with_string;

extern vector<PqlToken> valid_pattern_with_invalid_string;

extern vector<PqlToken> valid_pattern_with_string_and_whitespace;

extern vector<PqlToken> valid_pattern_with_wildcard_string;

extern vector<PqlToken> invalid_pattern_wildcard_string;

extern vector<PqlToken> invalid_pattern_while_parameters;

extern vector<PqlToken> invalid_pattern_if_parameters;

extern vector<PqlToken> valid_uses;

extern vector<PqlToken> valid_modifies;

extern vector<PqlToken> valid_follows;

extern vector<PqlToken> valid_parent;

extern vector<PqlToken> valid_follows_double_wildcard;

extern vector<PqlToken> valid_with;

extern vector<PqlToken> invalid_with_attrname_mismatch;

extern vector<PqlToken> valid_multi_with;

extern vector<PqlToken> valid_follows_a;

extern vector<PqlToken> valid_parent_a;

extern vector<PqlToken> valid_follows_a;

extern vector<PqlToken> valid_parent_a;

extern vector<PqlToken> valid_calls;

extern vector<PqlToken> valid_calls_a;

extern vector<PqlToken> valid_such_that_then_pattern;

extern vector<PqlToken> valid_pattern_then_such_that;

extern vector<PqlToken> valid_multi_pattern_with_such_that;

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

extern vector<PqlToken> invalid_with_ref_mismatch;

extern vector<PqlToken> invalid_with_parameter_type;

extern vector<PqlToken> invalid_with_undeclared_synonym;

extern vector<PqlToken> invalid_with_synonym_attrname;

extern vector<PqlToken> invalid_with_missing_dot;

extern vector<PqlToken> invalid_with_parameter_extra_tokens;
